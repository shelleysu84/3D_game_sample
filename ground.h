
#ifndef CGROUND_H_H
#define CGROUND_H_H
const int DEPTH=9;//7
const int N_MARKMAT=(1<<(DEPTH-1))+1;//pow(2,depth-1)+1;
const int nbyteOfmarkmat=sizeof(bool)*N_MARKMAT*N_MARKMAT;
class Cground:public Cmodel{
public:
	float width;//地形块宽度
	float height;//地形块高度
	float bmpHeight_div_height;//bmpheight/height
	float bmpWidth_div_width;//bmpwidth/width
	float winwidth,winheight;
	float ku,kv;//纹理缩放系数
	int depth;
	int n_markmat;
	float ymax;//地形上界
	float ymin;//地形下界
	float bx,bz;//左上角坐标
	float reso;//LOD分辨率
	bool markmat[N_MARKMAT][N_MARKMAT];//标记数组，用来标记各区域是否被分割
	bool*pMarkedElm[N_MARKMAT*N_MARKMAT];//指向markmat中被置1的元素，便于以后归零
	int top;//pMarkedElm上的栈顶指针
	Cfrustum*pfrustum;//视锥指针
	Cground(){
		pfrustum=NULL;
		ku=kv=1;
		top=0;
		depth=DEPTH;
		n_markmat=N_MARKMAT;
		//与vc6.0不同，vc2010不对类静态数组成员自动清零，所以要手动清零
		memset(markmat,0,nbyteOfmarkmat);

		width=0;
		height=0;
	    bmpHeight_div_height=0;
	    bmpWidth_div_width=0;
		bx=0;
		bz=0;
		reso=2000.0;


		ymax=0;
	    ymin=0;
	}
	void setKuKv(float _ku,float _kv){
		ku=_ku;
		kv=_kv;
	
	}
	void setReso(float _reso){//设置分辨率
		reso=_reso;

	}
	void linkFrustum(Cfrustum*_pfrustum){
		pfrustum=_pfrustum;
	}
	
	void fillNormalMat(const float landMat[BMPHEIGHT][BMPWIDTH],
		float normalMat[BMPHEIGHT][BMPWIDTH][3]
	)//根据高程数据landMat填充法向数据normalMat
	{
		if(BMPWIDTH!=BMPHEIGHT){
			cout<<"BMPWIDTH!=BMPHEIGHT"<<endl;
			system("pause");
			exit(0);
		}
		float dw=width/BMPWIDTH;
		float dh=height/BMPHEIGHT;
		
				for(int i=0;i<BMPHEIGHT;i++){
					for(int j=0;j<BMPWIDTH;j++){
						//填充normalMatMat[I][J][i][j]
						float hct=landMat[i][j];//本格高度(中心高度)
						float hrt;//右格高度
						float hdn;//下格高度
						if(j+1<BMPWIDTH){//j+1不越界
							hrt=landMat[i][j+1];
						}else{//j+1越界
							//绕回
							hrt=landMat[i][0];
						}
						if(i+1<BMPHEIGHT){//i+1不越界
							hdn=landMat[i+1][j];
						}else{//i+1越界
							//绕回
							hdn=landMat[0][j];
						}
						//      (0,hct,0)
						//         *-------> (w,hrt,0)
						//    vec1 |   vec2
						//        \|/
						//     (0,hdn,h)
						float vec1[4]={0,hdn-hct,dh,1};
						float vec2[4]={dw,hrt-hct,0,1};
						float norm[4];
						cross(vec1,vec2,norm);
						normalize(norm);
						normalMat[i][j][X]=norm[X];
						normalMat[i][j][Y]=norm[Y];
						normalMat[i][j][Z]=norm[Z];

					}
				}
			
	}
	void init(float _width,float _height,//地形尺寸
		float _bx,float _bz){
		width=_width;
		height=_height;
		bx=_bx;
		bz=_bz;

		if(width!=height){
			cout<<"width!=height"<<endl;
			system("pause");
			exit(0);
		}
		bmpWidth_div_width=(float)BMPWIDTH/width;
		bmpHeight_div_height=(float)BMPHEIGHT/height;

		
		//高度范围
		ymin=0;
		ymax=255*kH;
	}
	void setWinwidthAndWinheight(float _winwidth,float _winheight){
		winwidth=_winwidth;
		winheight=_winheight;
		
	}
	void show(){
		//先画地面再画地形,有利于z-rejection
		//-----------------------------画衬面
		{
			glBindTexture(GL_TEXTURE_2D,tex_water);
			const float h=2;
			const float d=5500;//3500;
			glBegin(GL_QUADS);
			glNormal3f(0,1,0);
			//v2(-d,h,-d)  v1(d,h,-d)
			//     ---------
			//     |       |
			//     |       |
			//     ---------
			//v3(-d,h,d)   v0(d,h,d)
			float camx=pfrustum->campos[X];
			float camz=pfrustum->campos[Z];
			glTexCoord2f(3,0);glVertex3f(d+camx,h,d+camz);
			glTexCoord2f(3,3);glVertex3f(d+camx,h,-d+camz);
			glTexCoord2f(0,3);glVertex3f(-d+camx,h,-d+camz);
			glTexCoord2f(0,0);glVertex3f(-d+camx,h,d+camz);
			
			glEnd();

		}
		//-----------------------画地形
		
		//清除四叉树上的标记
		for(int i=0;i<top;i++){
			*pMarkedElm[i]=0;
		}
		//top复位
		top=0;
	
		
		//渲染四叉树并对节点的分割情况作标记
		//绑定纹理
		glBindTexture(GL_TEXTURE_2D,texture);
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
		showAndMark(bx,bx+width,bz,bz+height,
			0,n_markmat-1,0,n_markmat-1);
		glEnd();
		//修补缝隙
		glBegin(GL_TRIANGLES);
		makeUp(bx,bx+width,bz,bz+height,
			0,n_markmat-1,0,n_markmat-1);
		glEnd();
		


	}
	void makeUp(float xmin,float xmax,float zmin,float zmax,
		int jmin,int jmax,int imin,int imax)
	{
		if(imin+1==imax){//是原子格，不用修补
			return;
		}else{
			//本区域中心
			int imid=(imin+imax)>>1;//除2
			int jmid=(jmin+jmax)>>1;
			//本区域中心坐标
			float xmid=(xmin+xmax)/2;
			float zmid=(zmin+zmax)/2;
				
			if(markmat[imid][jmid]){//如果区域中心mark，即本区域要细分
				
				//向下走
				//对四个孩子继续递归
				makeUp(xmin,xmid,zmin,zmid,
					jmin,jmid,imin,imid);
				makeUp(xmin,xmid,zmid,zmax,
					jmin,jmid,imid,imax);
				makeUp(xmid,xmax,zmid,zmax,
					jmid,jmax,imid,imax);
				makeUp(xmid,xmax,zmin,zmid,
					jmid,jmax,imin,imid);
				
			}else{
				
				//看上方同尺寸区域中心是否mark
				{
					int _imid=imid-(imax-imin);
					int _jmid=jmid;
					if(_imid>=0&&markmat[_imid][_jmid]){//上方同尺寸区域中心mark

						//用对三角面补漏
						//     p[2]
						//    /   \
						//  p[0]--p[1]
						//注意，都用3维向量，而不是4维
						float norm[3][3];
						float H0=getHAndNormal3(xmin,zmin,norm[0]);
						float H1=getHAndNormal3(xmax,zmin,norm[1]);
						float H2=getHAndNormal3(xmid,zmin,norm[2]);
						float p[3][3]={xmin,H0,zmin,
							xmax,H1,zmin,
							xmid,H2,zmin};
						float xminMulKu=xmin*ku;
						float xmaxMulKu=xmax*ku;
						float xmidMulKu=xmid*ku;
						float zminMulKv=zmin*kv;
						float u[3][2]={
							xminMulKu,zminMulKv,
							xmaxMulKu,zminMulKv,
							xmidMulKu,zminMulKv
						};
						//画
						{
							for(int i=0;i<3;i++){
								glNormal3fv(norm[i]);
								glTexCoord2fv(u[i]);
								glVertex3fv(p[i]);
							}
						}
					}
				}
				//看下方同尺寸区域中心是否mark
				{
					int _imid=imid+(imax-imin);
					int _jmid=jmid;
					if(_imid<N_MARKMAT&&markmat[_imid][_jmid]){//下方同尺寸区域中心mark

						//用对三角面补漏
						//  p[0]--p[2]
						//    \    /
						//     p[1]
						//注意，都用3维向量，而不是4维
						float norm[3][3];
						float H0=getHAndNormal3(xmin,zmax,norm[0]);
						float H1=getHAndNormal3(xmid,zmax,norm[1]);
						float H2=getHAndNormal3(xmax,zmax,norm[2]);
						float p[3][3]={xmin,H0,zmax,
							xmid,H1,zmax,
							xmax,H2,zmax};
						float xminMulKu=xmin*ku;
						float xmidMulKu=xmid*ku;
						float xmaxMulKu=xmax*ku;
						float zmaxMulKv=zmax*kv;
						float u[3][2]={
							xminMulKu,zmaxMulKv,
							xmidMulKu,zmaxMulKv,
							xmaxMulKu,zmaxMulKv
						};
						//画
						{
							for(int i=0;i<3;i++){
								glNormal3fv(norm[i]);
								glTexCoord2fv(u[i]);
								glVertex3fv(p[i]);
							}
						}
					}
				}
				//看左方同尺寸区域中心是否mark
				{
					int _imid=imid;
					int _jmid=jmid-(jmax-jmin);
					if(_jmid>=0&&markmat[_imid][_jmid]){//左方同尺寸区域中心mark

						//用对三角面补漏
						//        p[0]
						//      /  |
						//  p[1]   |
						//      \  |
						//        p[2]
						//注意，都用3维向量，而不是4维
						float norm[3][3];
						float H0=getHAndNormal3(xmin,zmin,norm[0]);
						float H1=getHAndNormal3(xmin,zmid,norm[1]);
						float H2=getHAndNormal3(xmin,zmax,norm[2]);
						float p[3][3]={xmin,H0,zmin,
							xmin,H1,zmid,
							xmin,H2,zmax};
						float xminMulKu=xmin*ku;
						float zminMulKv=zmin*kv;
						float zmidMulKv=zmid*kv;
						float zmaxMulKv=zmax*kv;
						float u[3][2]={
							xminMulKu,zminMulKv,
							xminMulKu,zmidMulKv,
							xminMulKu,zmaxMulKv
						};
						//画
						{
							for(int i=0;i<3;i++){
								glNormal3fv(norm[i]);
								glTexCoord2fv(u[i]);
								glVertex3fv(p[i]);
							}
						}
					}
				}
				//看右方同尺寸区域中心是否mark
				{
					int _imid=imid;
					int _jmid=jmid+(jmax-jmin);
					if(_jmid<N_MARKMAT&&markmat[_imid][_jmid]){//右方同尺寸区域中心mark

						//用对三角面补漏
						//        p[0]
						//         |  \
						//         |   p[2]
						//         |  /
						//        p[1]
						//注意，都用3维向量，而不是4维
						float norm[3][3];
						float H0=getHAndNormal3(xmax,zmin,norm[0]);
						float H1=getHAndNormal3(xmax,zmax,norm[1]);
						float H2=getHAndNormal3(xmax,zmid,norm[2]);
						float p[3][3]={xmax,H0,zmin,
							xmax,H1,zmax,
							xmax,H2,zmid};
						float xmaxMulKu=xmax*ku;
						float zminMulKv=zmin*kv;
						float zmaxMulKv=zmax*kv;
						float zmidMulKv=zmid*kv;
						float u[3][2]={
							xmaxMulKu,zminMulKv,
							xmaxMulKu,zmaxMulKv,
							xmaxMulKu,zmidMulKv
						};
						//画
						{
							for(int i=0;i<3;i++){
								glNormal3fv(norm[i]);
								glTexCoord2fv(u[i]);
								glVertex3fv(p[i]);
							}
						}

					}
				}

			}
		}
	
	}
/*	void showAndMark(float xmin,float xmax,float zmin,float zmax,
		int jmin,int jmax,int imin,int imax)
	{
		//检查当前节点是否与视截体相交
		int _ixmin,_izmin,_ixmax,_izmax;
		_ixmin=xToHeightmapSpace(xmin);
		_izmin=zToHeightmapSpace(zmin);
		_ixmax=xToHeightmapSpace(xmax);
		_izmax=zToHeightmapSpace(zmax);
		float H_min_min=getH_heightmapSpace(_ixmin,_izmin);
		float H_min_max=getH_heightmapSpace(_ixmin,_izmax);
		float H_max_max=getH_heightmapSpace(_ixmax,_izmax);
		float H_max_min=getH_heightmapSpace(_ixmax,_izmin);
		//求这几个点的包围盒
		float hmax=max(max(H_min_min,H_min_max),max(H_max_max,H_max_min));
		float hmin=min(min(H_min_min,H_min_max),min(H_max_max,H_max_min));
		//xmin,xmax,zmin,zmax,hmin,hmax构成包围盒
		//求最大边长
		float L=max(hmax-hmin,xmax-xmin);//由于xmin,xmax,zmin,zmax一定构是正方形，所以不用再考虑zmax-zmin
		//求包围球半径
		float R=L*0.86602540378443864;//sqrt(3)/2
		//求包围球球心
		float c[4]={(xmin+xmax)/2,(hmin+hmax)/2,(zmin+zmax)/2,1};
		//看c到planeList各面中，是否到某个面的距离为负且绝对值大于R，若是，则pNode不可见
		//否则认为可见
		bool visible=true;
		for(int i=0;i<=4
			;i++){//不考虑远平面
			Cplane&plane=planeList[i];
			//求c到plane的距离
			float PND=PND_point_plane(plane,c);
			if(PND<0&&-PND>R){
				visible=false;
				break;
			}
		}//得到visible
		if(visible){//如果可见
			//判断是否再分
			float xmid=(xmin+xmax)/2;
			float zmid=(zmin+zmax)/2;
			//求四边形中心(y=0)到视点的距离
			float c[4]={xmid,0,zmid,1};//四边形中心Y值直接用0，仅为了计算简单，很不精确。
			//指数值越大，LOD效应越明显
		   float d=pow2(campos[X]-c[X])
				+pow2(0.6*(campos[Y]-c[Y]))//Y乘以一个比1小的系数是为了让LOD对高度变化不敏感
				+pow2(campos[Z]-c[Z]);
			float e=xmax-xmin;//边长
			if(d<e*reso&&imin+1!=imax){//继续分
				int imid=(imin+imax)>>1;//除2
				int jmid=(jmin+jmax)>>1;
				markmat[imid][jmid]=true;
				pMarkedElm[top++]=&markmat[imid][jmid];
				//对四个孩子继续递归
				showAndMark(xmin,xmid,zmin,zmid,
					jmin,jmid,imin,imid);
				showAndMark(xmin,xmid,zmid,zmax,
					jmin,jmid,imid,imax);
				showAndMark(xmid,xmax,zmid,zmax,
					jmid,jmax,imid,imax);
				showAndMark(xmid,xmax,zmin,zmid,
					jmid,jmax,imin,imid);
			}else{//不分，画
				float norm[4][3];
				getNormal3_heightmapSpace(_ixmin,_izmin,norm[0]);
				getNormal3_heightmapSpace(_ixmin,_izmax,norm[1]);
				getNormal3_heightmapSpace(_ixmax,_izmax,norm[2]);
				getNormal3_heightmapSpace(_ixmax,_izmin,norm[3]);
				float p[4][3]={xmin,H_min_min,zmin,
						xmin,H_min_max,zmax,
						xmax,H_max_max,zmax,
						xmax,H_max_min,zmin};
				float xminMulKu=xmin*ku;
				float xmaxMulKu=xmax*ku;
				float zminMulKv=zmin*kv;
				float zmaxMulKv=zmax*kv;
				float u[4][2]={xminMulKu,zminMulKv,
							 xminMulKu,zmaxMulKv,
				             xmaxMulKu,zmaxMulKv,
				             xmaxMulKu,zminMulKv};
				//画
				{
					for(int i=0;i<4;i++){
						glNormal3fv(norm[i]);
						glTexCoord2fv(u[i]);
						glVertex3fv(p[i]);

					}
				
				}
		//		//画线框只用立即模式
		//		if(showMesh){
		//			const float color[3]={1,0,0};
		//			drawRectWire(p,color,1);
		//		}
			}
		}
	}//*/
	void showAndMark(float xmin,float xmax,float zmin,float zmax,
		int jmin,int jmax,int imin,int imax)
	{
		//检查当前节点是否与视截体相交	
		//求节点p所表示区域的保守包围盒
		//    上面
		//    p[0]--p[3]
		//     |     |
		//    p[1]--p[2]
		//    下面
		//    p[4]--p[7]
		//     |     |
		//    p[5]--p[6]
		float p[8][4];
		//上面
		init4(p[0],xmin,ymax,zmin,1);
		init4(p[1],xmin,ymax,zmax,1);
		init4(p[2],xmax,ymax,zmax,1);
		init4(p[3],xmax,ymax,zmin,1);
		//下面
		init4(p[4],xmin,ymin,zmin,1);
		init4(p[5],xmin,ymin,zmax,1);
		init4(p[6],xmax,ymin,zmax,1);
		init4(p[7],xmax,ymin,zmin,1);
		//
		//看p[0]~p[7]是否都在planeList中某个面的反面，如果是则可剔除
		bool visible=true;
		for(int i=0;i<=4;i++){//不考虑远平面
			Cplane&plane=pfrustum->planeList[i];
			//看p[0]~p[7]是否都在plane的背面
			bool allback=true;//假设都在背面
			for(int j=0;j<=7;j++){
				float side=PND_point_plane(plane,p[j]);
				if(side>0){
					allback=false;
					break;
				}
			}//得到allback
			if(allback){//如果全在背面
				//断定为不可见，不用再继续检测
				visible=false;
				break;
			}
		}//得到visible
		if(visible){//如果可见
			//判断是否再分
			float xmid=(xmin+xmax)/2;
			float zmid=(zmin+zmax)/2;
			//求四边形中心(y=0)到视点的距离
			float c[4]={xmid,0,zmid,1};//四边形中心Y值直接用0，仅为了计算简单，很不精确。
			//指数值越大，LOD效应越明显
		   float d=pow2(pfrustum->campos[X]-c[X])
				+pow2(0.6*(pfrustum->campos[Y]-c[Y]))//Y乘以一个比1小的系数是为了让LOD对高度变化不敏感
				+pow2(pfrustum->campos[Z]-c[Z]);
			float e=xmax-xmin;//边长
			if(d<e*reso&&imin+1!=imax){//继续分
				int imid=(imin+imax)>>1;//除2
				int jmid=(jmin+jmax)>>1;
				markmat[imid][jmid]=true;
				pMarkedElm[top++]=&markmat[imid][jmid];
				//对四个孩子继续递归
				showAndMark(xmin,xmid,zmin,zmid,
					jmin,jmid,imin,imid);
				showAndMark(xmin,xmid,zmid,zmax,
					jmin,jmid,imid,imax);
				showAndMark(xmid,xmax,zmid,zmax,
					jmid,jmax,imid,imax);
				showAndMark(xmid,xmax,zmin,zmid,
					jmid,jmax,imin,imid);
			}else{//不分，画
				int _ixmin,_izmin,_ixmax,_izmax;//xmin,zmin,xmax,zmax对应的heightmap空间坐标
				_ixmin=xToHeightmapSpace(xmin);
				_izmin=zToHeightmapSpace(zmin);
				_ixmax=xToHeightmapSpace(xmax);
				_izmax=zToHeightmapSpace(zmax);
				float norm[4][3];
				float H_min_min=getHAndNormal3_heightmapSpace(_ixmin,_izmin,norm[0]);
				float H_min_max=getHAndNormal3_heightmapSpace(_ixmin,_izmax,norm[1]);
				float H_max_max=getHAndNormal3_heightmapSpace(_ixmax,_izmax,norm[2]);
				float H_max_min=getHAndNormal3_heightmapSpace(_ixmax,_izmin,norm[3]);
				float p[4][3]={xmin,H_min_min,zmin,
						xmin,H_min_max,zmax,
						xmax,H_max_max,zmax,
						xmax,H_max_min,zmin};
				float xminMulKu=xmin*ku;
				float xmaxMulKu=xmax*ku;
				float zminMulKv=zmin*kv;
				float zmaxMulKv=zmax*kv;
				float u[4][2]={xminMulKu,zminMulKv,
							 xminMulKu,zmaxMulKv,
				             xmaxMulKu,zmaxMulKv,
				             xmaxMulKu,zminMulKv};
				//画
				{
					for(int i=0;i<4;i++){
						glNormal3fv(norm[i]);
						glTexCoord2fv(u[i]);
						glVertex3fv(p[i]);

					}
				
				}
		//		//画线框只用立即模式
		//		if(showMesh){
		//			const float color[3]={1,0,0};
		//			drawRectWire(p,color,1);
		//		}
			}
		}
	}//
	
	

	float getH(float x,float z)const {
		int _x,_z;
		_x=(x-bx)*bmpWidth_div_width;
		_z=(z-bz)*bmpHeight_div_height;
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			return landMat[_z][_x];
		}else{
			//		cout<<"waring:高度数组访问越界!"<<endl;
			return 0;

		}
	}
	int xToHeightmapSpace(float x)const {
		int _x=(x-bx)*bmpWidth_div_width;
		return _x;
	}
	int zToHeightmapSpace(float z)const {
		int _z=(z-bz)*bmpHeight_div_height;
		return _z;
	}
	float getH_heightmapSpace(int _x,int _z)const{//_x,_z均为heightmap空间的坐标
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			return landMat[_z][_x];
		}else{
			//		cout<<"waring:高度数组访问越界!"<<endl;
			return 0;

		}
	}
	void getNormal_heightmapSpace(int _x,int _z,float norm[4]){//_x,_z均为heightmap空间的坐标
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			norm[X]=normalMat[_z][_x][X];
			norm[Y]=normalMat[_z][_x][Y];
			norm[Z]=normalMat[_z][_x][Z];
			norm[W]=0;
		}else{
			init4(norm,0,1,0,0);
		//	cout<<"waring:高度数组访问越界!"<<endl;
			
		}

	}
	float getHAndNormal3_heightmapSpace(int _x,int _z,float norm[3]){
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			norm[X]=normalMat[_z][_x][X];
			norm[Y]=normalMat[_z][_x][Y];
			norm[Z]=normalMat[_z][_x][Z];
			return landMat[_z][_x];
		}else{
			norm[X]=0;norm[Y]=1;norm[Z]=0;
			//		cout<<"waring:高度数组访问越界!"<<endl;
			return 0;

		}
	}
	float getH(float x,float z,int&_x,int&_z)const {
		_x=(x-bx)*bmpWidth_div_width;
		_z=(z-bz)*bmpHeight_div_height;
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			return landMat[_z][_x];
		}else{
			//		cout<<"waring:高度数组访问越界!"<<endl;
			return 0;

		}
	}

	float getHAndNormal3(float x,float z,float norm[3]){
		int _x,_z;
		_x=(x-bx)*bmpWidth_div_width;
		_z=(z-bz)*bmpHeight_div_height;
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			norm[X]=normalMat[_z][_x][X];
			norm[Y]=normalMat[_z][_x][Y];
			norm[Z]=normalMat[_z][_x][Z];
			return landMat[_z][_x];
		}else{
			norm[X]=0;norm[Y]=1;norm[Z]=0;
			//		cout<<"waring:高度数组访问越界!"<<endl;
			return 0;

		}
	}
	
	bool getNormal(float x,float z,float norm[4]){
		int _x,_z;
		_x=(x-bx)*bmpWidth_div_width;
		_z=(z-bz)*bmpHeight_div_height;
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			norm[X]=normalMat[_z][_x][X];
			norm[Y]=normalMat[_z][_x][Y];
			norm[Z]=normalMat[_z][_x][Z];
			norm[W]=0;
			return true;
		}else{
			init4(norm,0,1,0,0);
		//	cout<<"waring:高度数组访问越界!"<<endl;
			return false;
			
		}

	}
	void getNormal3_heightmapSpace(int _x,int _z,float norm[3]){
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			norm[X]=normalMat[_z][_x][X];
			norm[Y]=normalMat[_z][_x][Y];
			norm[Z]=normalMat[_z][_x][Z];
		}else{
			norm[X]=0;
			norm[Y]=1;
			norm[Z]=0;
		//	cout<<"waring:高度数组访问越界!"<<endl;
		}

	}
	
};
#endif
