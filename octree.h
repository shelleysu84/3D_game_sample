#ifndef COCNODE_H_H
#define COCNODE_H_H
class CocNode{
public:
	float xmin,xmax,ymin,ymax,zmin,zmax;
	vector<CIDTriangle*> pIDtriList;
	CocNode*p[8];
	float c[4];//���ڵ������
	float R;//���ڵ�İ�Χ��뾶
	bool isLeaf;
	bool isSVLeaf;//�Ƿ�Ϊ��ӰҶ�ӽڵ㣨ע�⣬��ӰҶ�ӽڵ�����ǰ˲������м�ڵ㣩
	CSV SV;//��Ӱ��
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
//�˲���ʵ�ֶ�һ�������μ��Ͻ��пռ�����
#ifndef COCTREE_H_H
#define COCTREE_H_H
class Coctree:public Cmodel
{
public:
	CocNode*pRoot;//�˲�����ָ��
	vector<CocNode*> pVisibleLeafList;//�ɼ�Ҷ���б�
	vector<CocNode*> pSVvisibleLeafList;//�ɼ�����ӰҶ���б�
	vector<CIDTriangle*> pVisibleIDtriList;//�ɼ��������б�(�ɿɼ�Ҷ���б�������)
	Cmodel* pcell;
	Cfrustum*pfrustum;//��׶ָ��
	float xmin,xmax,ymin,ymax,zmin,zmax;//��Χ
	int leafCount;//Ҷ����
	int SVleafCount;//�ɼ���ӰҶ���б�
	vector<CIDTriangle*> pCollisionIDtriList;//��ײ���б�
	vector<CocNode*> pCollisionLeafList;//��ײҶ���б�
	Coctree(){
		SVleafCount=0;
		leafCount=0;
		pfrustum=NULL;
		pcell=NULL;
		pRoot=NULL;
	}
	~Coctree(){
		//�������нڵ�
		destoryAllNode(pRoot);
		//����pmesh
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
		//�����ٰ˸�����
		for(int i=0;i<8;i++){
			destoryAllNode(pNode->p[i]);
		}
		//�������Լ�
		delete pNode;
		
	}
	void markSVLeaf(){
		markSVLeaf_inn(pRoot);
	}
	void markSVLeaf_inn(CocNode*pNode){//�����ӰҶ��
		if(pNode==NULL)return;
		if(pNode->xmax-pNode->xmin<=300/*200/*400*/){//��ֵҪ��Ҷ�ӽڵ�Ĵ�
			pNode->setIsSVLeaf(true);
			return;
		}
		if((int)pNode->pIDtriList.size()<=60/*40/*80*/){//��ֵҪ��Ҷ�ӽڵ�Ĵ�
			pNode->setIsSVLeaf(true);
			return;
		}
		//ȥ����pNode�ĺ���
		for(int i=0;i<8;i++){
			markSVLeaf_inn(pNode->p[i]);
		}

	}
	void makeBondEdgeListAndSVAndSVBOForEachSVLeaf(float light_pos[4]){//Ϊ����ӰҶ�ӽڵ������߽���б����Ӱ��
		makeBondEdgeListAndSVAndSVBOForEachSVLeaf_inn(pRoot,light_pos);
	}
	void makeBondEdgeListAndSVAndSVBOForEachSVLeaf_inn(CocNode*&pNode,float light_pos[4]){
		if(pNode==NULL)return;
		if(pNode->isSVLeaf){//�������ӰҶ�ڵ㣬�������߽���б��shadow Volume��������
			//�ռ����ڵ���noSV=false����������������
			vector<CIDTriangle*> pIDtriList_haveSV;//���ڵ���Ҫ������Ӱ������������������б�
			{
				int nIDtri=pNode->pIDtriList.size();
				for(int i=0;i<nIDtri;i++){
					CIDTriangle*pIDtri=pNode->pIDtriList[i];
					if(pIDtri->noSV==false){
						pIDtriList_haveSV.push_back(pIDtri);
					}
				}
			}//�õ�pIDtriList_haveSV
			//������Ӱ��
			pNode->SV.makeSV(light_pos,pIDtriList_haveSV,pmesh->vlist);
			//Ϊ��Ӱ������vbo
			pNode->SV.makeVBO();
			//����
			return;
		}
		//�ݹ�
		for(int i=0;i<8;i++){
			makeBondEdgeListAndSVAndSVBOForEachSVLeaf_inn(pNode->p[i],light_pos);
			
		}
	}
	void pleafList2pIDtriList(const vector<CocNode*>&pleafList,vector<CIDTriangle*>&pIDtriList)
	//pleafList��pIDtriList��ת�������ڸ�leaf���ܰ�����ͬ��IDtri��������Ҫaddad���
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
		}//�õ�pVisibleIDtriListList
		//��pVisibleLeafList�и��ڵ�ĸ������ε�added��λ
		for(int i=0;i<nVL;i++){
			CocNode*pOcNode=pVisibleLeafList[i];
			int nIDtri=(int)pOcNode->pIDtriList.size();
			for(int j=0;j<nIDtri;j++){
				CIDTriangle&IDtri=*pOcNode->pIDtriList[j];
				IDtri.added=false;
			}
		}
		
	}
	void showTransp(){//��ʾ͸���棨�����ǽ����ʱ��һ���������Ϊ͸����
		//���pcellλ��
		float cellpos[4];
		pcell->getPosInFather(cellpos);
		glCullFace(GL_FRONT);//�����޳�
		///////////////////////////////   ����͸��������  ////////////////////////////////
		glColor4f(1,1,1,0.7);
		//�����ǽ��ʱ͸���������й���
		//������ӵ�ӽ����������б�
		get_pCollisionIDtriList(cellpos,pcell->Rc*3);//�ñ�pcell->Rc����İ뾶
		//-------------------------�Կɼ������ΰ�������з��ಢ��Ϊ����������
		pmesh->indexVBO.fillIndexArrayList(pCollisionIDtriList);//��pVisibleIDtriList���mesh.indexArrayList
		//-------------------------�ύ�������Դ棬�������Դ�
		pmesh->indexVBO.bind_arrayGbuffers();
		pmesh->indexVBO.submitIndexArrayList_and_showIndexGBufferList(pmesh->textureList);
		pmesh->indexVBO.unbind_arrayGBuffers();
		//-------------------------��ո�������
		pmesh->indexVBO.setTopListElmToZero();//�����������ջ��ָ�븴λ
		//////////////////////////     �ָ�״̬   ///////////////////////////////////////////
		glCullFace(GL_BACK);//�ָ�ΪϵͳĬ��ֵ
		
	}
	void showSolid(){//��ʾ��͸����
		glColor3f(1,1,1);
		//���pcellλ��
		float cellpos[4];
		pcell->getPosInFather(cellpos);
		glCullFace(GL_BACK);
		///////////////////////////////////////   ���ƿɼ�������  ///////////////////////////
		//-------------------------��ÿɼ�������
		getVisibleNodeList(pRoot);//�������׶�ཻ��Ҷ���б�
		pleafList2pIDtriList(pVisibleLeafList,pVisibleIDtriList);//��pVisibleLeafListת����pVisibleIDtriList
		//-------------------------�Կɼ������ΰ�������з��ಢ��Ϊ����������
		pmesh->indexVBO.fillIndexArrayList(pVisibleIDtriList);//��pVisibleIDtriList���mesh.indexArrayList
		//-------------------------�ύ�������Դ棬�������Դ�
		pmesh->indexVBO.bind_arrayGbuffers();
		pmesh->indexVBO.submitIndexArrayList_and_showIndexGBufferList(pmesh->textureList);
		pmesh->indexVBO.unbind_arrayGBuffers();
		//-------------------------��ո�������
		pmesh->indexVBO.setTopListElmToZero();//�����������ջ��ָ�븴λ		
		//-------------------------��տɼ�Ҷ�ӱ�Ϳɼ������α�
		pVisibleLeafList.clear();//��տɼ�Ҷ�ڵ��б�
		pVisibleIDtriList.clear();//��տɼ��������б�
	//	cout<<"pVisibleLeafList.capacity:"<<pVisibleLeafList.capacity()<<endl;
		//////////////////////////     ������Ӱ��ɼ���Ҷ���б�   ////////////////////////////
		pSVvisibleLeafList.clear();//����ϴν��
		getSVvisibleLeafList(cellpos,850/*800.0*/);//���pSVvisibleLeafList
		                                    //pSVvisibleLeafList���������ط�����
	}
	
	void setPcell(Cmodel*_pcell){
		pcell=_pcell;
	}
	void getVisibleNodeList(CocNode*pRoot)
		//��ÿɼ�Ҷ�ӽڵ��б�pVisibleLeafList
	{
		//��ÿɼ�Ҷ���б�
		getVisibleNodeList_inn(pRoot);
	}
	void getVisibleNodeList_inn(CocNode*pNode){
			if(pNode==NULL)return;
		//��鵱ǰ�ڵ��Ƿ����ӽ����ཻ
		const float *c=pNode->c;
		const float R=pNode->R;
			//��c��planeList�����У��Ƿ�ĳ����ľ���Ϊ���Ҿ���ֵ����R�����ǣ���pNode���ɼ�
			//������Ϊ�ɼ�
		bool visible=true;
		for(int i=0;i<=4;i++){
			Cplane&plane=pfrustum->planeList[i];
			//��c��plane�ľ���
			float PND=PND_point_plane(plane,c);
			if(PND<0&&-PND>R){
				visible=false;
				break;
			}
		}//�õ�visible
		if(visible){//����ɼ�
			//�����Ҷ�ӽڵ㣬����ӵ�pVsibleLeafList
			if(pNode->getIsLeaf()){
				pVisibleLeafList.push_back(pNode);
			}else{//��������ݹ�
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
		//�����cΪ����RΪ�뾶������ײ��Ҷ���б�
	{
		if(pNode==NULL)return;
		//�������cR�Ƿ���pNode��ײ
		bool collision=false;
		{
			const float *c_node=pNode->c;
			const float R_node=pNode->R;
			float d2=pow2(c_node[X]-c[X])+pow2(c_node[Y]-c[Y])+pow2(c_node[Z]-c[Z]);
			if(d2<pow2(R+R_node)){//�ཻ
				collision=true;
			}
		}//�õ�collision
		if(collision){//�����ײ
			if(pNode->isSVLeaf){
				//��pNode���뵽pSVvisibleLeafList��
				pSVvisibleLeafList.push_back(pNode);
			}else{
				//ȥ����pNode�ĺ���
				for(int i=0;i<8;i++){
					getSVvisibleLeafList_inn(pNode->p[i],c,R);
				}
			}
		}
	}
	void makeOctree()
		//��pmesh�����˲���
	{ 
		//--------------------�������ڵ�
		pRoot=new CocNode();
		//----��pmodel�ķ�Χ
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
		}//�õ�xmin,xmax,ymin,ymax,zmin,zmax
		float c[4]={(xmin+xmax)/2,(ymin+ymax)/2,(zmin+zmax)/2,1};//��Χ����
		//�����߳�
		float Lx=xmax-xmin;
		float Ly=ymax-ymin;
		float Lz=zmax-zmin;
		float Lmax=max(max(Lx,Ly),Lz);//���߳�
		const float margin=1;//��Щ����
		float dmax=Lmax/2+margin;//���ľ�
	
		//���¼���xmin,xmax,ymin,ymax,zmin,zmax��ʹ����Ϊһ��������
		xmin=c[X]-dmax;
		xmax=c[X]+dmax;
		ymin=c[Y]-dmax;
		ymax=c[Y]+dmax;
		zmin=c[Z]-dmax;
		zmax=c[Z]+dmax;//�õ�������
		cout<<"�˲����ռ䷶Χ"<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<zmin<<" "<<zmax<<endl;
		//------------------�������ڵ�
		pRoot->setSize(xmin,xmax,ymin,ymax,zmin,zmax);
		int nIDtri=(int)pmesh->IDtriList.size();
		pRoot->pIDtriList.resize(nIDtri);
		for(int i=0;i<nIDtri;i++){
			pRoot->pIDtriList[i]=&(pmesh->IDtriList[i]);
		}
		
		//------------------�������ӽڵ�
		makeOctree_inn(pRoot);
		//------------------�����˲�����Ϊ���ڵ����c��R
		renderCR(pRoot);
		//------------------��˲���Ҷ����
		calLeafCount(pRoot,leafCount);
		//------------------�����ӰҶ�ڵ�
		markSVLeaf();
		//------------------Ϊÿ����ӰҶ�ڵ����ɹ�Դ��������Ӱ�壨SV����SV��vbo
		makeBondEdgeListAndSVAndSVBOForEachSVLeaf(light_pos);
		//------------------�������Ҷ�ڵ���������б�
		deletepIDtriListForEachNode(pRoot);
		//------------------����ӰҶ�ӽڵ���
		calSVleafCount(pRoot,SVleafCount);
		//------------------Ϊ���б����ռ�
		reserveSpace_pVisibleLeafList();
		reserveSpace_pSVvisibleLeafList();
		reserveSpace_pVisibleIDtriList();
		reserveSpace_pCollisionLeafList();
		reserveSpace_pCollisionIDtriList();



		
	}
	void deletepIDtriListForEachNode(CocNode*pNode){
		if(pNode==NULL)return;//�������䣬��Ϊ���ǽ���Ҷ���Ƴ���NULL��
							  //������Ȼÿ���ڵ㶼�ض���Ҷ�����ӣ�
							  //�������п���ΪNULL��
		if(pNode->getIsLeaf()){
			return;
		}
		//ɾ��pNode->pIDtriList
		{
			vector<CIDTriangle*>(pNode->pIDtriList).swap(pNode->pIDtriList);
		}
		//���µݹ�
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
	void renderCR(CocNode*&pNode){//�����ڵ��c��R
		if(pNode==NULL)return;
		//���c
		init4(pNode->c,(pNode->xmin+pNode->xmax)/2,
			(pNode->ymin+pNode->ymax)/2,
			(pNode->zmin+pNode->zmax)/2,
			1);
		//���R
		pNode->R=(pNode->xmax-pNode->xmin)*0.86602540378443864;//sqrt(3)/2
		//�ݹ�
		for(int i=0;i<8;i++){
			renderCR(pNode->p[i]);
			
		}
	}
	void get_pCollisionIDtriList(float c[4],float R){
		pCollisionLeafList.clear();
		pCollisionIDtriList.clear();
		get_pCollisionLeafList(pRoot,c,R);
		//��pCollisionLeafList����pCollisionIDtriList
		//����pCollisionLeafList��Ҷ���е��������ռ����������н��г��أ��õ�pCollisionIDtriList
		int nCollisionLeaf=(int)pCollisionLeafList.size();
		for(int k=0;k<nCollisionLeaf;k++)
		{
			CocNode*pNode=pCollisionLeafList[k];
			//��pNode�е���������ӵ�
			int nIDtri=(int)pNode->pIDtriList.size();
			for(int i=0;i<nIDtri;i++){
				if(pNode->pIDtriList[i]->added==false){
					pCollisionIDtriList.push_back(pNode->pIDtriList[i]);
					pNode->pIDtriList[i]->added=true;
				}
			}
		}
		//�õ�pCollisionIDtriList
		//��pCollisionIDtriList�и��������added��λ
		int nCollisionIDtri=(int)pCollisionIDtriList.size();
		for(int i=0;i<nCollisionIDtri;i++){
			pCollisionIDtriList[i]->added=false;
		}
	}
	void get_pCollisionLeafList(CocNode*pNode,float c[4],float R)
		//�����cΪ����RΪ�뾶�������ײҶ�ӽڵ�
	{
		if(pNode==NULL)return;
		//�������cR�Ƿ���pNode��ײ
		bool collision=false;
		{
			const float *c_node=pNode->c;
			const float R_node=pNode->R;
			float d2=pow2(c_node[X]-c[X])+pow2(c_node[Y]-c[Y])+pow2(c_node[Z]-c[Z]);
			if(d2<pow2(R+R_node)){//�ཻ
				collision=true;
			}
		}//�õ�collision
		if(collision){//�����ײ
			if(pNode->getIsLeaf()){
				//��pNode���뵽pCollisionLeafList
				pCollisionLeafList.push_back(pNode);
			}else{
				//ȥ����pNode�ĺ���
				for(int i=0;i<8;i++){
					get_pCollisionLeafList(pNode->p[i],c,R);
				}
			}
		}
	}
	void makeOctree_inn(CocNode*&pNode){//ΪpNode�����˸����ӽڵ�
		//�ݹ�ֹͣ����
		//�⿿�ڵ����������Ʋ����Ա������޷ָ���Ա�����ڵ㶨һ���ߴ�����
		if(pNode->xmax-pNode->xmin<=30.0){
			pNode->setIsLeaf(true);
			return;
		}
		if((int)pNode->pIDtriList.size()<=8){
			pNode->setIsLeaf(true);
			return;
		}
		//-------------------���ɰ˸����ӽڵ�
		float xmin=pNode->xmin;
		float xmax=pNode->xmax;
		float ymin=pNode->ymin;
		float ymax=pNode->ymax;
		float zmin=pNode->zmin;
		float zmax=pNode->zmax;
		float xmid=(xmin+xmax)/2;
		float ymid=(ymin+ymax)/2;
		float zmid=(zmin+zmax)/2;
		float L=(xmax-xmin)/2;//pNode��ʾ��������߳�
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
			//�ж�pNode->pIDtriList����Щ��������pocNode�ཻ
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
			}//�õ�pocNode->pIDtriList
			//���pocNode->pIDtriListΪ�գ���pocNodeӦɾ��
			if((int)pocNode->pIDtriList.size()==0){
				delete pocNode;
			}else{//����
				//�������������ڵ���
				pNode->p[i]=pocNode;
				//�����ݹ�
				makeOctree_inn(pocNode);
				
			}
		}
	}
	bool intersect_IDTri_AABBBox(CIDTriangle&IDtri,
		float xmin,float xmax,float ymin,float ymax,float zmin,float zmax){//�ж��������Ƿ���AABB���ཻ
			float*v0=pmesh->vlist[IDtri.vID[0]].v;
			float*v1=pmesh->vlist[IDtri.vID[1]].v;
			float*v2=pmesh->vlist[IDtri.vID[2]].v;
			//�ж����������Ƿ���AABB��ĳһ��������
			//�Ƿ�������X��С��xmin
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