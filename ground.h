
#ifndef CGROUND_H_H
#define CGROUND_H_H
const int DEPTH=9;//7
const int N_MARKMAT=(1<<(DEPTH-1))+1;//pow(2,depth-1)+1;
const int nbyteOfmarkmat=sizeof(bool)*N_MARKMAT*N_MARKMAT;
class Cground:public Cmodel{
public:
	float width;//���ο���
	float height;//���ο�߶�
	float bmpHeight_div_height;//bmpheight/height
	float bmpWidth_div_width;//bmpwidth/width
	float winwidth,winheight;
	float ku,kv;//��������ϵ��
	int depth;
	int n_markmat;
	float ymax;//�����Ͻ�
	float ymin;//�����½�
	float bx,bz;//���Ͻ�����
	float reso;//LOD�ֱ���
	bool markmat[N_MARKMAT][N_MARKMAT];//������飬������Ǹ������Ƿ񱻷ָ�
	bool*pMarkedElm[N_MARKMAT*N_MARKMAT];//ָ��markmat�б���1��Ԫ�أ������Ժ����
	int top;//pMarkedElm�ϵ�ջ��ָ��
	Cfrustum*pfrustum;//��׶ָ��
	Cground(){
		pfrustum=NULL;
		ku=kv=1;
		top=0;
		depth=DEPTH;
		n_markmat=N_MARKMAT;
		//��vc6.0��ͬ��vc2010�����ྲ̬�����Ա�Զ����㣬����Ҫ�ֶ�����
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
	void setReso(float _reso){//���÷ֱ���
		reso=_reso;

	}
	void linkFrustum(Cfrustum*_pfrustum){
		pfrustum=_pfrustum;
	}
	
	void fillNormalMat(const float landMat[BMPHEIGHT][BMPWIDTH],
		float normalMat[BMPHEIGHT][BMPWIDTH][3]
	)//���ݸ߳�����landMat��䷨������normalMat
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
						//���normalMatMat[I][J][i][j]
						float hct=landMat[i][j];//����߶�(���ĸ߶�)
						float hrt;//�Ҹ�߶�
						float hdn;//�¸�߶�
						if(j+1<BMPWIDTH){//j+1��Խ��
							hrt=landMat[i][j+1];
						}else{//j+1Խ��
							//�ƻ�
							hrt=landMat[i][0];
						}
						if(i+1<BMPHEIGHT){//i+1��Խ��
							hdn=landMat[i+1][j];
						}else{//i+1Խ��
							//�ƻ�
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
	void init(float _width,float _height,//���γߴ�
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

		
		//�߶ȷ�Χ
		ymin=0;
		ymax=255*kH;
	}
	void setWinwidthAndWinheight(float _winwidth,float _winheight){
		winwidth=_winwidth;
		winheight=_winheight;
		
	}
	void show(){
		//�Ȼ������ٻ�����,������z-rejection
		//-----------------------------������
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
		//-----------------------������
		
		//����Ĳ����ϵı��
		for(int i=0;i<top;i++){
			*pMarkedElm[i]=0;
		}
		//top��λ
		top=0;
	
		
		//��Ⱦ�Ĳ������Խڵ�ķָ���������
		//������
		glBindTexture(GL_TEXTURE_2D,texture);
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
		showAndMark(bx,bx+width,bz,bz+height,
			0,n_markmat-1,0,n_markmat-1);
		glEnd();
		//�޲���϶
		glBegin(GL_TRIANGLES);
		makeUp(bx,bx+width,bz,bz+height,
			0,n_markmat-1,0,n_markmat-1);
		glEnd();
		


	}
	void makeUp(float xmin,float xmax,float zmin,float zmax,
		int jmin,int jmax,int imin,int imax)
	{
		if(imin+1==imax){//��ԭ�Ӹ񣬲����޲�
			return;
		}else{
			//����������
			int imid=(imin+imax)>>1;//��2
			int jmid=(jmin+jmax)>>1;
			//��������������
			float xmid=(xmin+xmax)/2;
			float zmid=(zmin+zmax)/2;
				
			if(markmat[imid][jmid]){//�����������mark����������Ҫϸ��
				
				//������
				//���ĸ����Ӽ����ݹ�
				makeUp(xmin,xmid,zmin,zmid,
					jmin,jmid,imin,imid);
				makeUp(xmin,xmid,zmid,zmax,
					jmin,jmid,imid,imax);
				makeUp(xmid,xmax,zmid,zmax,
					jmid,jmax,imid,imax);
				makeUp(xmid,xmax,zmin,zmid,
					jmid,jmax,imin,imid);
				
			}else{
				
				//���Ϸ�ͬ�ߴ����������Ƿ�mark
				{
					int _imid=imid-(imax-imin);
					int _jmid=jmid;
					if(_imid>=0&&markmat[_imid][_jmid]){//�Ϸ�ͬ�ߴ���������mark

						//�ö������油©
						//     p[2]
						//    /   \
						//  p[0]--p[1]
						//ע�⣬����3ά������������4ά
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
						//��
						{
							for(int i=0;i<3;i++){
								glNormal3fv(norm[i]);
								glTexCoord2fv(u[i]);
								glVertex3fv(p[i]);
							}
						}
					}
				}
				//���·�ͬ�ߴ����������Ƿ�mark
				{
					int _imid=imid+(imax-imin);
					int _jmid=jmid;
					if(_imid<N_MARKMAT&&markmat[_imid][_jmid]){//�·�ͬ�ߴ���������mark

						//�ö������油©
						//  p[0]--p[2]
						//    \    /
						//     p[1]
						//ע�⣬����3ά������������4ά
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
						//��
						{
							for(int i=0;i<3;i++){
								glNormal3fv(norm[i]);
								glTexCoord2fv(u[i]);
								glVertex3fv(p[i]);
							}
						}
					}
				}
				//����ͬ�ߴ����������Ƿ�mark
				{
					int _imid=imid;
					int _jmid=jmid-(jmax-jmin);
					if(_jmid>=0&&markmat[_imid][_jmid]){//��ͬ�ߴ���������mark

						//�ö������油©
						//        p[0]
						//      /  |
						//  p[1]   |
						//      \  |
						//        p[2]
						//ע�⣬����3ά������������4ά
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
						//��
						{
							for(int i=0;i<3;i++){
								glNormal3fv(norm[i]);
								glTexCoord2fv(u[i]);
								glVertex3fv(p[i]);
							}
						}
					}
				}
				//���ҷ�ͬ�ߴ����������Ƿ�mark
				{
					int _imid=imid;
					int _jmid=jmid+(jmax-jmin);
					if(_jmid<N_MARKMAT&&markmat[_imid][_jmid]){//�ҷ�ͬ�ߴ���������mark

						//�ö������油©
						//        p[0]
						//         |  \
						//         |   p[2]
						//         |  /
						//        p[1]
						//ע�⣬����3ά������������4ά
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
						//��
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
		//��鵱ǰ�ڵ��Ƿ����ӽ����ཻ
		int _ixmin,_izmin,_ixmax,_izmax;
		_ixmin=xToHeightmapSpace(xmin);
		_izmin=zToHeightmapSpace(zmin);
		_ixmax=xToHeightmapSpace(xmax);
		_izmax=zToHeightmapSpace(zmax);
		float H_min_min=getH_heightmapSpace(_ixmin,_izmin);
		float H_min_max=getH_heightmapSpace(_ixmin,_izmax);
		float H_max_max=getH_heightmapSpace(_ixmax,_izmax);
		float H_max_min=getH_heightmapSpace(_ixmax,_izmin);
		//���⼸����İ�Χ��
		float hmax=max(max(H_min_min,H_min_max),max(H_max_max,H_max_min));
		float hmin=min(min(H_min_min,H_min_max),min(H_max_max,H_max_min));
		//xmin,xmax,zmin,zmax,hmin,hmax���ɰ�Χ��
		//�����߳�
		float L=max(hmax-hmin,xmax-xmin);//����xmin,xmax,zmin,zmaxһ�����������Σ����Բ����ٿ���zmax-zmin
		//���Χ��뾶
		float R=L*0.86602540378443864;//sqrt(3)/2
		//���Χ������
		float c[4]={(xmin+xmax)/2,(hmin+hmax)/2,(zmin+zmax)/2,1};
		//��c��planeList�����У��Ƿ�ĳ����ľ���Ϊ���Ҿ���ֵ����R�����ǣ���pNode���ɼ�
		//������Ϊ�ɼ�
		bool visible=true;
		for(int i=0;i<=4
			;i++){//������Զƽ��
			Cplane&plane=planeList[i];
			//��c��plane�ľ���
			float PND=PND_point_plane(plane,c);
			if(PND<0&&-PND>R){
				visible=false;
				break;
			}
		}//�õ�visible
		if(visible){//����ɼ�
			//�ж��Ƿ��ٷ�
			float xmid=(xmin+xmax)/2;
			float zmid=(zmin+zmax)/2;
			//���ı�������(y=0)���ӵ�ľ���
			float c[4]={xmid,0,zmid,1};//�ı�������Yֱֵ����0����Ϊ�˼���򵥣��ܲ���ȷ��
			//ָ��ֵԽ��LODЧӦԽ����
		   float d=pow2(campos[X]-c[X])
				+pow2(0.6*(campos[Y]-c[Y]))//Y����һ����1С��ϵ����Ϊ����LOD�Ը߶ȱ仯������
				+pow2(campos[Z]-c[Z]);
			float e=xmax-xmin;//�߳�
			if(d<e*reso&&imin+1!=imax){//������
				int imid=(imin+imax)>>1;//��2
				int jmid=(jmin+jmax)>>1;
				markmat[imid][jmid]=true;
				pMarkedElm[top++]=&markmat[imid][jmid];
				//���ĸ����Ӽ����ݹ�
				showAndMark(xmin,xmid,zmin,zmid,
					jmin,jmid,imin,imid);
				showAndMark(xmin,xmid,zmid,zmax,
					jmin,jmid,imid,imax);
				showAndMark(xmid,xmax,zmid,zmax,
					jmid,jmax,imid,imax);
				showAndMark(xmid,xmax,zmin,zmid,
					jmid,jmax,imin,imid);
			}else{//���֣���
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
				//��
				{
					for(int i=0;i<4;i++){
						glNormal3fv(norm[i]);
						glTexCoord2fv(u[i]);
						glVertex3fv(p[i]);

					}
				
				}
		//		//���߿�ֻ������ģʽ
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
		//��鵱ǰ�ڵ��Ƿ����ӽ����ཻ	
		//��ڵ�p����ʾ����ı��ذ�Χ��
		//    ����
		//    p[0]--p[3]
		//     |     |
		//    p[1]--p[2]
		//    ����
		//    p[4]--p[7]
		//     |     |
		//    p[5]--p[6]
		float p[8][4];
		//����
		init4(p[0],xmin,ymax,zmin,1);
		init4(p[1],xmin,ymax,zmax,1);
		init4(p[2],xmax,ymax,zmax,1);
		init4(p[3],xmax,ymax,zmin,1);
		//����
		init4(p[4],xmin,ymin,zmin,1);
		init4(p[5],xmin,ymin,zmax,1);
		init4(p[6],xmax,ymin,zmax,1);
		init4(p[7],xmax,ymin,zmin,1);
		//
		//��p[0]~p[7]�Ƿ���planeList��ĳ����ķ��棬���������޳�
		bool visible=true;
		for(int i=0;i<=4;i++){//������Զƽ��
			Cplane&plane=pfrustum->planeList[i];
			//��p[0]~p[7]�Ƿ���plane�ı���
			bool allback=true;//���趼�ڱ���
			for(int j=0;j<=7;j++){
				float side=PND_point_plane(plane,p[j]);
				if(side>0){
					allback=false;
					break;
				}
			}//�õ�allback
			if(allback){//���ȫ�ڱ���
				//�϶�Ϊ���ɼ��������ټ������
				visible=false;
				break;
			}
		}//�õ�visible
		if(visible){//����ɼ�
			//�ж��Ƿ��ٷ�
			float xmid=(xmin+xmax)/2;
			float zmid=(zmin+zmax)/2;
			//���ı�������(y=0)���ӵ�ľ���
			float c[4]={xmid,0,zmid,1};//�ı�������Yֱֵ����0����Ϊ�˼���򵥣��ܲ���ȷ��
			//ָ��ֵԽ��LODЧӦԽ����
		   float d=pow2(pfrustum->campos[X]-c[X])
				+pow2(0.6*(pfrustum->campos[Y]-c[Y]))//Y����һ����1С��ϵ����Ϊ����LOD�Ը߶ȱ仯������
				+pow2(pfrustum->campos[Z]-c[Z]);
			float e=xmax-xmin;//�߳�
			if(d<e*reso&&imin+1!=imax){//������
				int imid=(imin+imax)>>1;//��2
				int jmid=(jmin+jmax)>>1;
				markmat[imid][jmid]=true;
				pMarkedElm[top++]=&markmat[imid][jmid];
				//���ĸ����Ӽ����ݹ�
				showAndMark(xmin,xmid,zmin,zmid,
					jmin,jmid,imin,imid);
				showAndMark(xmin,xmid,zmid,zmax,
					jmin,jmid,imid,imax);
				showAndMark(xmid,xmax,zmid,zmax,
					jmid,jmax,imid,imax);
				showAndMark(xmid,xmax,zmin,zmid,
					jmid,jmax,imin,imid);
			}else{//���֣���
				int _ixmin,_izmin,_ixmax,_izmax;//xmin,zmin,xmax,zmax��Ӧ��heightmap�ռ�����
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
				//��
				{
					for(int i=0;i<4;i++){
						glNormal3fv(norm[i]);
						glTexCoord2fv(u[i]);
						glVertex3fv(p[i]);

					}
				
				}
		//		//���߿�ֻ������ģʽ
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
			//		cout<<"waring:�߶��������Խ��!"<<endl;
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
	float getH_heightmapSpace(int _x,int _z)const{//_x,_z��Ϊheightmap�ռ������
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			return landMat[_z][_x];
		}else{
			//		cout<<"waring:�߶��������Խ��!"<<endl;
			return 0;

		}
	}
	void getNormal_heightmapSpace(int _x,int _z,float norm[4]){//_x,_z��Ϊheightmap�ռ������
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			norm[X]=normalMat[_z][_x][X];
			norm[Y]=normalMat[_z][_x][Y];
			norm[Z]=normalMat[_z][_x][Z];
			norm[W]=0;
		}else{
			init4(norm,0,1,0,0);
		//	cout<<"waring:�߶��������Խ��!"<<endl;
			
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
			//		cout<<"waring:�߶��������Խ��!"<<endl;
			return 0;

		}
	}
	float getH(float x,float z,int&_x,int&_z)const {
		_x=(x-bx)*bmpWidth_div_width;
		_z=(z-bz)*bmpHeight_div_height;
		if(_x>=0&&_x<BMPWIDTH&&_z>=0&&_z<BMPHEIGHT){
			return landMat[_z][_x];
		}else{
			//		cout<<"waring:�߶��������Խ��!"<<endl;
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
			//		cout<<"waring:�߶��������Խ��!"<<endl;
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
		//	cout<<"waring:�߶��������Խ��!"<<endl;
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
		//	cout<<"waring:�߶��������Խ��!"<<endl;
		}

	}
	
};
#endif
