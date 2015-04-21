
inline void renderBox(const float c[4],const float R){//��ȾcΪ���ģ����ľ�ΪR��ƽ�����������������
		float x_add_R=c[X]+R;
		float x_sub_R=c[X]-R;
		float y_add_R=c[Y]+R;
		float y_sub_R=c[Y]-R;
		float z_add_R=c[Z]+R;
		float z_sub_R=c[Z]-R;
		//��������ϵ��������İ˸���������
		float p[8][4];
		//�µ���
		//   p0      p3
		//    -------
		//    |     |
		//    -------
		//   p1      p2
		init4(p[0],x_sub_R,y_sub_R,z_sub_R,1);
		init4(p[1],x_sub_R,y_sub_R,z_add_R,1);
		init4(p[2],x_add_R,y_sub_R,z_add_R,1);
		init4(p[3],x_add_R,y_sub_R,z_sub_R,1);
		//�ϵ���
		//   p4      p7
		//    -------
		//    |     |
		//    -------
		//   p5      p6
		init4(p[4],x_sub_R,y_add_R,z_sub_R,1);
		init4(p[5],x_sub_R,y_add_R,z_add_R,1);
		init4(p[6],x_add_R,y_add_R,z_add_R,1);
		init4(p[7],x_add_R,y_add_R,z_sub_R,1);
		//����Χ��
		//ǰ��2651
		glBegin(GL_QUADS);
		glVertex3fv(p[2]);
		glVertex3fv(p[6]);
		glVertex3fv(p[5]);
		glVertex3fv(p[1]);
		//����0473
		glVertex3fv(p[0]);
		glVertex3fv(p[4]);
		glVertex3fv(p[7]);
		glVertex3fv(p[3]);
		//����1540
		glVertex3fv(p[1]);
		glVertex3fv(p[5]);
		glVertex3fv(p[4]);
		glVertex3fv(p[0]);
		//����3762
		glVertex3fv(p[3]);
		glVertex3fv(p[7]);
		glVertex3fv(p[6]);
		glVertex3fv(p[2]);
		//����4567
		glVertex3fv(p[4]);
		glVertex3fv(p[5]);
		glVertex3fv(p[6]);
		glVertex3fv(p[7]);
		//����2103
		glVertex3fv(p[2]);
		glVertex3fv(p[1]);
		glVertex3fv(p[0]);
		glVertex3fv(p[3]);
		glEnd();
	}
inline bool doOccQuery(const float pos[4],const float R,
		GLuint pixCountThreshold=0//��Ⱦ������<=pixCountThreshold��Ϊ�ڵ�
		)//�����Ƿ��ڵ���������Ұ�⣩
	{
		glBeginQueryARB( GL_SAMPLES_PASSED_ARB, occQuery );
		renderBox(pos,R);
		glEndQueryARB( GL_SAMPLES_PASSED_ARB );
		//��ý��
		GLuint count=0; 
		glGetQueryObjectuivARB( occQuery, GL_QUERY_RESULT_ARB, &count);
		if(count<=pixCountThreshold){
			return true;
		}else{
			return false;
		}
	}
inline void printMat(float mat[16]){
	for(int i=0;i<16;i++){
		cout<<mat[i]<<" ";
	}cout<<endl;

}
inline bool isPointEq(const float p0[4],const float p1[4]){//�ж������Ƿ��غ�
		const float localEps=0.0001;//�ѳ���10
		if(fabs(p0[X]-p1[X])<localEps
			&&fabs(p0[Y]-p1[Y])<localEps
			&&fabs(p0[Z]-p1[Z])<localEps){
			return true;
		}else{
			return false;
		}
	}

inline	bool getNormalOfTri(const float p0[4],const float p1[4],const float p2[4],float norm[4]){//���������ķ�������ǰ�˷����Ƿ񹹳�������
		float p01[4],p02[4];
		sub((float(&)[4])(*p1),(float(&)[4])(*p0),p01);
		sub((float(&)[4])(*p2),(float(&)[4])(*p0),p02);
		cross(p01,p02,norm);
		if(normalize(norm)==0){//���nromģΪ0��˵�����㹲��
			return false;
		}
		return true;
	}

inline bool isInSight_small(float c[4],float R,
	GLdouble modelMatrix[16],
	GLdouble projMatrix[16]){//�ж���cΪ���ģ����ľ�ΪR(R<<��׵)����ƽ������������������Ƿ����ӽ�����
	GLint viewport[4]={0,0,widthEx,height};
	float x=c[X];
	float y=c[Y];
	float z=c[Z];
	//��������ϵ��������İ˸���������
	float p[8][4];
	//�µ���
	init4(p[0],x-R,y-R,z-R,1);
	init4(p[1],x-R,y-R,z+R,1);
	init4(p[2],x+R,y-R,z+R,1);
	init4(p[3],x+R,y-R,z-R,1);
	//�ϵ���
	init4(p[4],x-R,y+R,z-R,1);
	init4(p[5],x-R,y+R,z+R,1);
	init4(p[6],x+R,y+R,z+R,1);
	init4(p[7],x+R,y+R,z-R,1);
	//��pͶӰ����Ļ�ռ䣬����û�������ӽ������ߣ�����R<<��׶�����Բ����ܳ��������������׶����������Ե��ҽ����ж���������׶��ʱ����׶�ཻ
	//����ֻ�ж϶����Ƿ�����׶�ڣ�����Ҫ�����˹�ϵ��ȷ������������������У��
	for(int i=0;i<=7;i++){
		GLdouble winx,winy,winz;
		gluProject(p[i][X],p[i][Y],p[i][Z],modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
		//�ж�(winx,winy,winz)�Ƿ�����Ļ�ռ���
		if(winx>=viewport[0]&&winx<=viewport[0]+viewport[2]
			&&winy>=viewport[1]&&winy<=viewport[1]+viewport[3]
			&&winz>=0&&winz<=1){//����Ļ�ռ���
			return true;
		}
	}
	return false;
}

inline void makeShadowStencil_zpass(vector<Cv4>&vlist,float light_pos[4],float campos[4])//������vlist������Ӱģ��
//����vlist�е������Ϊ��������
//�˺����ĵ��û�����
//��������ϣ�������Ȳ��ԣ���дz-buff���ر������رչ��գ��ر�color_material���ر���������
//	glEnable(GL_STENCIL_TEST);
//	glDepthMask(GL_FALSE);
//  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
//	glDisable(GL_TEXTURE_2D);
//  glDisable(GL_LIGHTING);
//  glDisable(GL_FOG);
{
	
	//��light_posΪ���Ķ�vlist���зŴ�
	vector<Cv4> vlist2=vlist;//vlist2Ϊ�Ŵ���
	int n=vlist.size();
	for(int i=0;i<n;i++){
		float *v=vlist[i].v;
		float *v2=vlist2[i].v;
		sub((float(&)[4])(*v),(float(&)[4])(*light_pos),(float(&)[4])(*v2));//mark!
		mul(/*1000*/100,v2,v2);
		add(light_pos,v2,v2);
		v2[W]=1;
	}//�õ�vlist2

	glStencilFunc(GL_ALWAYS,1,1);

	//��������
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	glCullFace(GL_BACK);
	glBegin(GL_QUADS);
	for(int i=0;i<n;i++){
		int inext=(i+1)%n;
		float *v1=vlist[i].v;
		float *v2=vlist[inext].v;
		float *_v1=vlist2[i].v;
		float *_v2=vlist2[inext].v;
		//    light_pos
		//     /   \
		//    v1---v2
		//   /       \
		// _v1-------_v2
		glVertex3fv(v1);
		glVertex3fv(_v1);
		glVertex3fv(_v2);
		glVertex3fv(v2);	
	}
	glEnd();

	//��������
	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
	glCullFace(GL_FRONT);
	glBegin(GL_QUADS);
	for(int i=0;i<n;i++){
		int inext=(i+1)%n;
		float *v1=vlist[i].v;
		float *v2=vlist[inext].v;
		float *_v1=vlist2[i].v;
		float *_v2=vlist2[inext].v;
		//    light_pos
		//     /   \
		//    v1---v2
		//   /       \
		// _v1-------_v2
		glVertex3fv(v1);
		glVertex3fv(_v1);
		glVertex3fv(_v2);
		glVertex3fv(v2);	
	}
	glEnd();
	glCullFace(GL_BACK);//�ָ�ΪϵͳĬ��ֵBACK

		

}
inline void renderStencilShadow(float alpha,float _width,float _height){//��Ⱦģ����Ӱ
	//����ӿڲ���
	GLint viewport[4]={0,0,_width,_height};
	//��Ⱦ�ӿ�
	glStencilFunc(GL_LESS,defaultStencilValue,~0);//defaultStencilValueС��ģ��ֵ�Ĳ��ֲ���ͨ��������Ⱦ
	                                             
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	Begin2D(MATH_COOR,0,0,_width,_height);
	glColor4f(0,0,0,alpha);//0.25
	glBegin(GL_QUADS);
	glVertex2f(viewport[0],viewport[1]);
	glVertex2f(viewport[0]+viewport[2],viewport[1]);
	glVertex2f(viewport[0]+viewport[2],viewport[1]+viewport[3]);
	glVertex2f(viewport[0],viewport[1]+viewport[3]);
	glEnd();
	End2D();
}


inline	float PND_point_plane(const Cplane&plane,const float point[4]){//�ǵ㵽ƽ����������
		const float *norm=plane.norm;
		const float *p=plane.p;
		float ppoint[4];
		sub((float(&)[4])(*point),(float(&)[4])(*p),ppoint);
		float PNd=dot(norm,ppoint);
		return PNd;
		
}
inline void calLinearModel(const float x0,const float y0,//����1
						   const float x1,const float y1,//����2
						   float&a,float&b//�����ϵ��
						   )
//�������ģ��
// ������     X        Y
//-----------------------------
// ���ݣ�     x0       y0
//            x1       y1
//-----------------------------
// ģ�ͣ� Y=aX+b
//-----------------------------
// ��ã� a=(y1-y0)/(x1-x0)
//        b=(x1y0-x0y1)/(x1-x0)
//*********************??????????????????????���������ִ��ʱ����0��?????????
{
	a=(y1-y0)/(x1-x0);
	b=(x1*y0-x0*y1)/(x1-x0);
}
inline void readLandMat(){
	//��ȡ�߳�����
		const int MAKEUP=(4-(BMPWIDTH*3)%4)%4;
		//data��_dataֻ��������ʱ��ת�õ����飬������뾲̬���飬��ʹջ�ռ䲻���ã�
		//��������������붯̬�ռ�
		BYTE *data=new BYTE[BMPHEIGHT*(BMPWIDTH*3+MAKEUP)];//λͼ���ݻ���
		BYTE *_data=new BYTE[BMPHEIGHT*BMPWIDTH];//��ȡ���ݵ�����
		
	
			string pathName="data\\land\\land.bmp";
				//��ȡλͼ
				FILE  *infile=NULL;
				//���ļ�
				errno_t eno=fopen_s(&infile,pathName.c_str(),"rb");
				if(eno!=0)
				{ 
					printf("Open 24 bit bitmap file failed!"); 
					return;
				}
					//���ļ�ͷ
				BYTE  Head[54];
				fread(Head,54,1,infile); 
				//������
				
				
			int len=BMPHEIGHT*(BMPWIDTH*3+MAKEUP);
				fread(data,len,1,infile);
				fclose(infile);
					//�����ļ�ͷ
				int bmpWidth =Head[18]+Head[19]*256;
				int bmpHeight=Head[22]+Head[23]*256;
				if(bmpWidth!=BMPWIDTH||bmpHeight!=BMPHEIGHT){
					cout<<"���ò�ƥ��!"<<endl;
					exit(0);
					return ;
				}
				int makeup=(4-(bmpWidth*3)%4)%4;
				if(makeup!=MAKEUP){
					cout<<"���ò�ƥ��!"<<endl;
					exit(0);
					return ;
				}
	
		//��ȡ����
				int p=0;
				int _p=0;
				for(int i=0;i<bmpHeight;i++){
					for(int j=0;j<bmpWidth;j++){
					
						_data[_p++]=data[p];//blue ��Ϊbmp��ͨ��˳����bgr
						
						p+=3;
					}
					p+=MAKEUP;
				}
					//��_data��䵽landMat������
				_p=0;
				for(int i=BMPHEIGHT-1;i>=0;i--){//����
					for(int j=0;j<BMPWIDTH;j++){//����
						landMat[i][j]=kH*_data[_p++];
					}
				}

				
	
				delete []_data;
				delete []data;
}

inline bool IsExtensionSupported( char* szTargetExtension ) 
{ 
    const unsigned char *pszExtensions = NULL; 
    const unsigned char *pszStart; 
    unsigned char *pszWhere, *pszTerminator; 
  
    // Extension names should not have spaces 
    pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' ); 
    if( pszWhere || *szTargetExtension == '\0' ) 
        return false; 
  
    // Get Extensions String 
    pszExtensions = glGetString( GL_EXTENSIONS ); 
  
    // Search The Extensions String For An Exact Copy 
    pszStart = pszExtensions; 
    for(;;) 
    { 
        pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension ); 
        if( !pszWhere ) 
            break; 
        pszTerminator = pszWhere + strlen( szTargetExtension ); 
        if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' ) 
            if( *pszTerminator == ' ' || *pszTerminator == '\0' ) 
                return true; 
        pszStart = pszTerminator; 
    } 
    return false; 
} 
inline void softMov(const float originPos[4],//��ʼλ��
	const float tgPos[4],//Ŀ��λ��
	const float percentage,//�ƶ��ٷֱ�
	float rsPos[4]//���λ��
	)//ת�ƶ�
{
	//��originPos�Ƶ�tgPos�������Ч������һ�ε�λ��ֻ�ƶ�percentage�ı���
	float vec[4];//��originPos��tgPos������
	sub((float(&)[4])(*tgPos),(float(&)[4])(*originPos),vec);
	float dvec[4];//ʵ���ƶ�����
	mul(percentage,vec,dvec);//�õ�dvec
	//�ƶ�
	add(originPos,dvec,rsPos);
}



			
