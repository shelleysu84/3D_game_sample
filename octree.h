#ifndef COCNODE_H_H
#define COCNODE_H_H
class CocNode{
public:
	float xmin,xmax,ymin,ymax,zmin,zmax;
	vector<CIDTriangle*> pIDtriList;
	CocNode*p[8];
	float c[4];//本节点的中心
	float R;//本节点的包围球半径
	bool isLeaf;
	bool isSVLeaf;//是否为阴影叶子节点（注意，阴影叶子节点可能是八叉树的中间节点）
	CSV SV;//阴影体
	void setSize(float _xmin,float _xmax,float _ymin,float _ymax,float _zmin,float _zmax){
		xmin=_xmin;
		xmax=_xmax;
		ymin=_ymin;
		ymax=_ymax;
		zmin=_zmin;
		zmax=_zmax;
		for(int i=0;i<8;i++){
			p[i]=NULL;
		}
		isLeaf=false;
		isSVLeaf=false;
	}
	void addPIDtri(CIDTriangle*pIDtri){
		pIDtriList.push_back(pIDtri);
	}
	bool getIsLeaf()const{
		return isLeaf;
	}
	void setIsLeaf(bool _isLeaf){
		isLeaf=_isLeaf;
	}
	bool getIsSVLeaf()const{
		return isSVLeaf;
	}
	void setIsSVLeaf(bool _isSVLeaf){
		isSVLeaf=_isSVLeaf;
	}
	
};
#endif
//八叉树实现对一个三角形集合进行空间索引
#ifndef COCTREE_H_H
#define COCTREE_H_H
class Coctree:public Cmodel
{
public:
	CocNode*pRoot;//八叉树根指针
	vector<CocNode*> pVisibleLeafList;//可见叶子列表
	vector<CocNode*> pSVvisibleLeafList;//可见的阴影叶子列表
	vector<CIDTriangle*> pVisibleIDtriList;//可见三角面列表(由可见叶子列表计算得来)
	Cmodel* pcell;
	Cfrustum*pfrustum;//视锥指针
	float xmin,xmax,ymin,ymax,zmin,zmax;//范围
	int leafCount;//叶子数
	int SVleafCount;//可见阴影叶子列表
	vector<CIDTriangle*> pCollisionIDtriList;//碰撞面列表
	vector<CocNode*> pCollisionLeafList;//碰撞叶子列表
	Coctree(){
		SVleafCount=0;
		leafCount=0;
		pfrustum=NULL;
		pcell=NULL;
		pRoot=NULL;
	}
	~Coctree(){
		//销毁所有节点
		destoryAllNode(pRoot);
		//销毁pmesh
		delete pmesh;
	}
	void reserveSpace_pCollisionLeafList(){
		pCollisionLeafList.reserve(leafCount);
	}
	void reserveSpace_pCollisionIDtriList(){
		pCollisionIDtriList.reserve((int)pmesh->IDtriList.size());
	}
	void reserveSpace_pVisibleLeafList(){
		pVisibleLeafList.reserve(leafCount);
	}
	void reserveSpace_pSVvisibleLeafList(){
		pSVvisibleLeafList.reserve(SVleafCount);
	}
	void reserveSpace_pVisibleIDtriList(){
		pVisibleIDtriList.reserve((int)pmesh->IDtriList.size());
	}
	void linkFrustum(Cfrustum*_pfrustum){
		pfrustum=_pfrustum;
	}
	
	void destoryAllNode(CocNode*pNode){
		if(pNode==NULL)return;
		//先销毁八个孩子
		for(int i=0;i<8;i++){
			destoryAllNode(pNode->p[i]);
		}
		//后销毁自己
		delete pNode;
		
	}
	void markSVLeaf(){
		markSVLeaf_inn(pRoot);
	}
	void markSVLeaf_inn(CocNode*pNode){//标记阴影叶子
		if(pNode==NULL)return;
		if(pNode->xmax-pNode->xmin<=300/*200/*400*/){//此值要比叶子节点的大
			pNode->setIsSVLeaf(true);
			return;
		}
		if((int)pNode->pIDtriList.size()<=60/*40/*80*/){//此值要比叶子节点的大
			pNode->setIsSVLeaf(true);
			return;
		}
		//去考察pNode的孩子
		for(int i=0;i<8;i++){
			markSVLeaf_inn(pNode->p[i]);
		}

	}
	void makeBondEdgeListAndSVAndSVBOForEachSVLeaf(float light_pos[4]){//为各阴影叶子节点制作边界边列表和阴影体
		makeBondEdgeListAndSVAndSVBOForEachSVLeaf_inn(pRoot,light_pos);
	}
	void makeBondEdgeListAndSVAndSVBOForEachSVLeaf_inn(CocNode*&pNode,float light_pos[4]){
		if(pNode==NULL)return;
		if(pNode->isSVLeaf){//如果是阴影叶节点，则制作边界边列表和shadow Volume，并返回
			//收集本节点上noSV=false的所有索引三角形
			vector<CIDTriangle*> pIDtriList_haveSV;//本节点上要生成阴影体的所有索引三角形列表
			{
				int nIDtri=pNode->pIDtriList.size();
				for(int i=0;i<nIDtri;i++){
					CIDTriangle*pIDtri=pNode->pIDtriList[i];
					if(pIDtri->noSV==false){
						pIDtriList_haveSV.push_back(pIDtri);
					}
				}
			}//得到pIDtriList_haveSV
			//制作阴影体
			pNode->SV.makeSV(light_pos,pIDtriList_haveSV,pmesh->vlist);
			//为阴影体生成vbo
			pNode->SV.makeVBO();
			//返回
			return;
		}
		//递归
		for(int i=0;i<8;i++){
			makeBondEdgeListAndSVAndSVBOForEachSVLeaf_inn(pNode->p[i],light_pos);
			
		}
	}
	void pleafList2pIDtriList(const vector<CocNode*>&pleafList,vector<CIDTriangle*>&pIDtriList)
	//pleafList向pIDtriList的转化，由于各leaf可能包含相同的IDtri，所以需要addad标记
	{
		int nVL=(int)pVisibleLeafList.size();
		for(int i=0;i<nVL;i++){
			CocNode*pOcNode=pVisibleLeafList[i];
			int nIDtri=(int)pOcNode->pIDtriList.size();
			for(int j=0;j<nIDtri;j++){
				CIDTriangle&IDtri=*pOcNode->pIDtriList[j];
				if(IDtri.added)continue;
				IDtri.added=true;
				pVisibleIDtriList.push_back(&IDtri);
			}
		}//得到pVisibleIDtriListList
		//将pVisibleLeafList中各节点的各三角形的added复位
		for(int i=0;i<nVL;i++){
			CocNode*pOcNode=pVisibleLeafList[i];
			int nIDtri=(int)pOcNode->pIDtriList.size();
			for(int j=0;j<nIDtri;j++){
				CIDTriangle&IDtri=*pOcNode->pIDtriList[j];
				IDtri.added=false;
			}
		}
		
	}
	void showTransp(){//显示透明面（相机在墙壁内时将一部分面绘制为透明）
		//获得pcell位置
		float cellpos[4];
		pcell->getPosInFather(cellpos);
		glCullFace(GL_FRONT);//正面剔除
		///////////////////////////////   绘制透视三角形  ////////////////////////////////
		glColor4f(1,1,1,0.7);
		//相机在墙内时透视三角形有贡献
		//获得与视点接近的三角面列表
		get_pCollisionIDtriList(cellpos,pcell->Rc*3);//用比pcell->Rc更大的半径
		//-------------------------对可见三角形按纹理进行分类并存为顶点索引表
		pmesh->indexVBO.fillIndexArrayList(pCollisionIDtriList);//用pVisibleIDtriList填充mesh.indexArrayList
		//-------------------------提交索引表到显存，并绘制显存
		pmesh->indexVBO.bind_arrayGbuffers();
		pmesh->indexVBO.submitIndexArrayList_and_showIndexGBufferList(pmesh->textureList);
		pmesh->indexVBO.unbind_arrayGBuffers();
		//-------------------------清空各索引表
		pmesh->indexVBO.setTopListElmToZero();//将各索引表的栈顶指针复位
		//////////////////////////     恢复状态   ///////////////////////////////////////////
		glCullFace(GL_BACK);//恢复为系统默认值
		
	}
	void showSolid(){//显示不透明面
		glColor3f(1,1,1);
		//获得pcell位置
		float cellpos[4];
		pcell->getPosInFather(cellpos);
		glCullFace(GL_BACK);
		///////////////////////////////////////   绘制可见三角形  ///////////////////////////
		//-------------------------获得可见三角形
		getVisibleNodeList(pRoot);//获得与视锥相交的叶子列表
		pleafList2pIDtriList(pVisibleLeafList,pVisibleIDtriList);//将pVisibleLeafList转化成pVisibleIDtriList
		//-------------------------对可见三角形按纹理进行分类并存为顶点索引表
		pmesh->indexVBO.fillIndexArrayList(pVisibleIDtriList);//用pVisibleIDtriList填充mesh.indexArrayList
		//-------------------------提交索引表到显存，并绘制显存
		pmesh->indexVBO.bind_arrayGbuffers();
		pmesh->indexVBO.submitIndexArrayList_and_showIndexGBufferList(pmesh->textureList);
		pmesh->indexVBO.unbind_arrayGBuffers();
		//-------------------------清空各索引表
		pmesh->indexVBO.setTopListElmToZero();//将各索引表的栈顶指针复位		
		//-------------------------清空可见叶子表和可见三角形表
		pVisibleLeafList.clear();//清空可见叶节点列表
		pVisibleIDtriList.clear();//清空可见三角形列表
	//	cout<<"pVisibleLeafList.capacity:"<<pVisibleLeafList.capacity()<<endl;
		//////////////////////////     计算阴影体可见的叶子列表   ////////////////////////////
		pSVvisibleLeafList.clear();//清空上次结果
		getSVvisibleLeafList(cellpos,850/*800.0*/);//获得pSVvisibleLeafList
		                                    //pSVvisibleLeafList将在其它地方绘制
	}
	
	void setPcell(Cmodel*_pcell){
		pcell=_pcell;
	}
	void getVisibleNodeList(CocNode*pRoot)
		//获得可见叶子节点列表pVisibleLeafList
	{
		//获得可见叶子列表
		getVisibleNodeList_inn(pRoot);
	}
	void getVisibleNodeList_inn(CocNode*pNode){
			if(pNode==NULL)return;
		//检查当前节点是否与视截体相交
		const float *c=pNode->c;
		const float R=pNode->R;
			//看c到planeList各面中，是否到某个面的距离为负且绝对值大于R，若是，则pNode不可见
			//否则认为可见
		bool visible=true;
		for(int i=0;i<=4;i++){
			Cplane&plane=pfrustum->planeList[i];
			//求c到plane的距离
			float PND=PND_point_plane(plane,c);
			if(PND<0&&-PND>R){
				visible=false;
				break;
			}
		}//得到visible
		if(visible){//如果可见
			//如果是叶子节点，则添加到pVsibleLeafList
			if(pNode->getIsLeaf()){
				pVisibleLeafList.push_back(pNode);
			}else{//否则继续递归
				for(int i=0;i<8;i++){
					getVisibleNodeList_inn(pNode->p[i]);
				}
			}

		}
	}
	void getSVvisibleLeafList(float c[4],float R){
		getSVvisibleLeafList_inn(pRoot,c,R);
	}
	void getSVvisibleLeafList_inn(CocNode*pNode,float c[4],float R)
		//获得与c为球心R为半径球体碰撞的叶子列表
	{
		if(pNode==NULL)return;
		//检查球体cR是否与pNode碰撞
		bool collision=false;
		{
			const float *c_node=pNode->c;
			const float R_node=pNode->R;
			float d2=pow2(c_node[X]-c[X])+pow2(c_node[Y]-c[Y])+pow2(c_node[Z]-c[Z]);
			if(d2<pow2(R+R_node)){//相交
				collision=true;
			}
		}//得到collision
		if(collision){//如果碰撞
			if(pNode->isSVLeaf){
				//将pNode加入到pSVvisibleLeafList中
				pSVvisibleLeafList.push_back(pNode);
			}else{
				//去考察pNode的孩子
				for(int i=0;i<8;i++){
					getSVvisibleLeafList_inn(pNode->p[i],c,R);
				}
			}
		}
	}
	void makeOctree()
		//对pmesh建立八叉树
	{ 
		//--------------------建立根节点
		pRoot=new CocNode();
		//----求pmodel的范围
		xmin=inf;
		xmax=-inf;
		ymin=inf;
		ymax=-inf;
		zmin=inf;
		zmax=-inf;
		{
			int nv=(int)pmesh->vlist.size();
			for(int i=0;i<nv;i++){
				float *v=pmesh->vlist[i].v;
				if(v[X]<xmin)xmin=v[X];
				if(v[X]>xmax)xmax=v[X];
				if(v[Y]<ymin)ymin=v[Y];
				if(v[Y]>ymax)ymax=v[Y];
				if(v[Z]<zmin)zmin=v[Z];
				if(v[Z]>zmax)zmax=v[Z];
			}
		}//得到xmin,xmax,ymin,ymax,zmin,zmax
		float c[4]={(xmin+xmax)/2,(ymin+ymax)/2,(zmin+zmax)/2,1};//范围中心
		//求最大边长
		float Lx=xmax-xmin;
		float Ly=ymax-ymin;
		float Lz=zmax-zmin;
		float Lmax=max(max(Lx,Ly),Lz);//最大边长
		const float margin=1;//留些余量
		float dmax=Lmax/2+margin;//边心距
	
		//重新计算xmin,xmax,ymin,ymax,zmin,zmax，使区域为一个正方体
		xmin=c[X]-dmax;
		xmax=c[X]+dmax;
		ymin=c[Y]-dmax;
		ymax=c[Y]+dmax;
		zmin=c[Z]-dmax;
		zmax=c[Z]+dmax;//得到正方体
		cout<<"八叉树空间范围"<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<zmin<<" "<<zmax<<endl;
		//------------------建立根节点
		pRoot->setSize(xmin,xmax,ymin,ymax,zmin,zmax);
		int nIDtri=(int)pmesh->IDtriList.size();
		pRoot->pIDtriList.resize(nIDtri);
		for(int i=0;i<nIDtri;i++){
			pRoot->pIDtriList[i]=&(pmesh->IDtriList[i]);
		}
		
		//------------------建立孩子节点
		makeOctree_inn(pRoot);
		//------------------遍历八叉树，为各节点填充c和R
		renderCR(pRoot);
		//------------------求八叉树叶子数
		calLeafCount(pRoot,leafCount);
		//------------------标记阴影叶节点
		markSVLeaf();
		//------------------为每个阴影叶节点生成光源轮廓和阴影体（SV）及SV的vbo
		makeBondEdgeListAndSVAndSVBOForEachSVLeaf(light_pos);
		//------------------清除各非叶节点的三角面列表
		deletepIDtriListForEachNode(pRoot);
		//------------------求阴影叶子节点数
		calSVleafCount(pRoot,SVleafCount);
		//------------------为各列表分配空间
		reserveSpace_pVisibleLeafList();
		reserveSpace_pSVvisibleLeafList();
		reserveSpace_pVisibleIDtriList();
		reserveSpace_pCollisionLeafList();
		reserveSpace_pCollisionIDtriList();



		
	}
	void deletepIDtriListForEachNode(CocNode*pNode){
		if(pNode==NULL)return;//必须加这句，因为我们将空叶子制成了NULL，
							  //所以虽然每个节点都必定有叶个孩子，
							  //但其中有可能为NULL者
		if(pNode->getIsLeaf()){
			return;
		}
		//删除pNode->pIDtriList
		{
			vector<CIDTriangle*>(pNode->pIDtriList).swap(pNode->pIDtriList);
		}
		//向下递归
		for(int i=0;i<8;i++){
			deletepIDtriListForEachNode(pNode->p[i]);
		}
		
	}
	void calLeafCount(CocNode*pNode,int&leafCount){
		if(pNode==NULL)return;
		if(pNode->getIsLeaf()){
			leafCount++;
		}
		for(int i=0;i<8;i++){
			calLeafCount(pNode->p[i],leafCount);
		}
	}
	void calSVleafCount(CocNode*pNode,int&SVleafCount){
		if(pNode==NULL)return;
		if(pNode->getIsSVLeaf()){
			SVleafCount++;
		}
		for(int i=0;i<8;i++){
			calSVleafCount(pNode->p[i],SVleafCount);
		}
	}
	void renderCR(CocNode*&pNode){//填充各节点的c和R
		if(pNode==NULL)return;
		//填充c
		init4(pNode->c,(pNode->xmin+pNode->xmax)/2,
			(pNode->ymin+pNode->ymax)/2,
			(pNode->zmin+pNode->zmax)/2,
			1);
		//填充R
		pNode->R=(pNode->xmax-pNode->xmin)*0.86602540378443864;//sqrt(3)/2
		//递归
		for(int i=0;i<8;i++){
			renderCR(pNode->p[i]);
			
		}
	}
	void get_pCollisionIDtriList(float c[4],float R){
		pCollisionLeafList.clear();
		pCollisionIDtriList.clear();
		get_pCollisionLeafList(pRoot,c,R);
		//由pCollisionLeafList计算pCollisionIDtriList
		//即将pCollisionLeafList中叶子中的三角面收集起来（其中进行除重）得到pCollisionIDtriList
		int nCollisionLeaf=(int)pCollisionLeafList.size();
		for(int k=0;k<nCollisionLeaf;k++)
		{
			CocNode*pNode=pCollisionLeafList[k];
			//将pNode中的三角面添加到
			int nIDtri=(int)pNode->pIDtriList.size();
			for(int i=0;i<nIDtri;i++){
				if(pNode->pIDtriList[i]->added==false){
					pCollisionIDtriList.push_back(pNode->pIDtriList[i]);
					pNode->pIDtriList[i]->added=true;
				}
			}
		}
		//得到pCollisionIDtriList
		//将pCollisionIDtriList中各三角面的added复位
		int nCollisionIDtri=(int)pCollisionIDtriList.size();
		for(int i=0;i<nCollisionIDtri;i++){
			pCollisionIDtriList[i]->added=false;
		}
	}
	void get_pCollisionLeafList(CocNode*pNode,float c[4],float R)
		//获得与c为球心R为半径球体的碰撞叶子节点
	{
		if(pNode==NULL)return;
		//检查球体cR是否与pNode碰撞
		bool collision=false;
		{
			const float *c_node=pNode->c;
			const float R_node=pNode->R;
			float d2=pow2(c_node[X]-c[X])+pow2(c_node[Y]-c[Y])+pow2(c_node[Z]-c[Z]);
			if(d2<pow2(R+R_node)){//相交
				collision=true;
			}
		}//得到collision
		if(collision){//如果碰撞
			if(pNode->getIsLeaf()){
				//将pNode加入到pCollisionLeafList
				pCollisionLeafList.push_back(pNode);
			}else{
				//去考察pNode的孩子
				for(int i=0;i<8;i++){
					get_pCollisionLeafList(pNode->p[i],c,R);
				}
			}
		}
	}
	void makeOctree_inn(CocNode*&pNode){//为pNode建立八个孩子节点
		//递归停止条件
		//光靠节点内面数限制不足以避免无限分割，所以必须给节点定一个尺寸下限
		if(pNode->xmax-pNode->xmin<=30.0){
			pNode->setIsLeaf(true);
			return;
		}
		if((int)pNode->pIDtriList.size()<=8){
			pNode->setIsLeaf(true);
			return;
		}
		//-------------------生成八个孩子节点
		float xmin=pNode->xmin;
		float xmax=pNode->xmax;
		float ymin=pNode->ymin;
		float ymax=pNode->ymax;
		float zmin=pNode->zmin;
		float zmax=pNode->zmax;
		float xmid=(xmin+xmax)/2;
		float ymid=(ymin+ymax)/2;
		float zmid=(zmin+zmax)/2;
		float L=(xmax-xmin)/2;//pNode表示的正方体边长
		float range[8][6]={
			{xmin,xmin+L,ymin,ymin+L,zmin,zmin+L},
			{xmid,xmid+L,ymin,ymin+L,zmin,zmin+L},
			{xmin,xmin+L,ymid,ymid+L,zmin,zmin+L},
			{xmid,xmid+L,ymid,ymid+L,zmin,zmin+L},
			{xmin,xmin+L,ymin,ymin+L,zmid,zmid+L},
			{xmid,xmid+L,ymin,ymin+L,zmid,zmid+L},
			{xmin,xmin+L,ymid,ymid+L,zmid,zmid+L},
			{xmid,xmid+L,ymid,ymid+L,zmid,zmid+L}
		};
		for(int i=0;i<8;i++){
			CocNode*pocNode=new CocNode();
			pocNode->setSize(range[i][0],range[i][1],
				range[i][2],range[i][3],
				range[i][4],range[i][5]);
			//判断pNode->pIDtriList中哪些三角形与pocNode相交
			int nIDtri=(int)pNode->pIDtriList.size();
			for(int j=0;j<nIDtri;j++){
				CIDTriangle&IDtri=*pNode->pIDtriList[j];
				bool intersect=intersect_IDTri_AABBBox(IDtri,
					pocNode->xmin,pocNode->xmax,
					pocNode->ymin,pocNode->ymax,
					pocNode->zmin,pocNode->zmax);
				if(intersect){
					pocNode->addPIDtri(&IDtri);
				}
			}//得到pocNode->pIDtriList
			//如果pocNode->pIDtriList为空，则pocNode应删除
			if((int)pocNode->pIDtriList.size()==0){
				delete pocNode;
			}else{//否则
				//将孩子连到父节点上
				pNode->p[i]=pocNode;
				//继续递归
				makeOctree_inn(pocNode);
				
			}
		}
	}
	bool intersect_IDTri_AABBBox(CIDTriangle&IDtri,
		float xmin,float xmax,float ymin,float ymax,float zmin,float zmax){//判断三角形是否与AABB盒相交
			float*v0=pmesh->vlist[IDtri.vID[0]].v;
			float*v1=pmesh->vlist[IDtri.vID[1]].v;
			float*v2=pmesh->vlist[IDtri.vID[2]].v;
			//判断三个顶点是否都在AABB盒某一个面的外侧
			//是否三顶点X都小于xmin
			if(v0[X]<xmin&&v1[X]<xmin&&v2[X]<xmin)return false;
			if(v0[X]>xmax&&v1[X]>xmax&&v2[X]>xmax)return false;
			if(v0[Y]<ymin&&v1[Y]<ymin&&v2[Y]<ymin)return false;
			if(v0[Y]>ymax&&v1[Y]>ymax&&v2[Y]>ymax)return false;
			if(v0[Z]<zmin&&v1[Z]<zmin&&v2[Z]<zmin)return false;
			if(v0[Z]>zmax&&v1[Z]>zmax&&v2[Z]>zmax)return false;
			return true;
	}

};
#endif