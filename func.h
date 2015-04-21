
inline void renderBox(const float c[4],const float R){//渲染c为中心，边心距为R，平行于坐标轴的立方体
		float x_add_R=c[X]+R;
		float x_sub_R=c[X]-R;
		float y_add_R=c[Y]+R;
		float y_sub_R=c[Y]-R;
		float z_add_R=c[Z]+R;
		float z_sub_R=c[Z]-R;
		//世界坐标系内立方体的八个顶点坐标
		float p[8][4];
		//下底面
		//   p0      p3
		//    -------
		//    |     |
		//    -------
		//   p1      p2
		init4(p[0],x_sub_R,y_sub_R,z_sub_R,1);
		init4(p[1],x_sub_R,y_sub_R,z_add_R,1);
		init4(p[2],x_add_R,y_sub_R,z_add_R,1);
		init4(p[3],x_add_R,y_sub_R,z_sub_R,1);
		//上底面
		//   p4      p7
		//    -------
		//    |     |
		//    -------
		//   p5      p6
		init4(p[4],x_sub_R,y_add_R,z_sub_R,1);
		init4(p[5],x_sub_R,y_add_R,z_add_R,1);
		init4(p[6],x_add_R,y_add_R,z_add_R,1);
		init4(p[7],x_add_R,y_add_R,z_sub_R,1);
		//画包围盒
		//前面2651
		glBegin(GL_QUADS);
		glVertex3fv(p[2]);
		glVertex3fv(p[6]);
		glVertex3fv(p[5]);
		glVertex3fv(p[1]);
		//后面0473
		glVertex3fv(p[0]);
		glVertex3fv(p[4]);
		glVertex3fv(p[7]);
		glVertex3fv(p[3]);
		//左面1540
		glVertex3fv(p[1]);
		glVertex3fv(p[5]);
		glVertex3fv(p[4]);
		glVertex3fv(p[0]);
		//右面3762
		glVertex3fv(p[3]);
		glVertex3fv(p[7]);
		glVertex3fv(p[6]);
		glVertex3fv(p[2]);
		//上面4567
		glVertex3fv(p[4]);
		glVertex3fv(p[5]);
		glVertex3fv(p[6]);
		glVertex3fv(p[7]);
		//下面2103
		glVertex3fv(p[2]);
		glVertex3fv(p[1]);
		glVertex3fv(p[0]);
		glVertex3fv(p[3]);
		glEnd();
	}
inline bool doOccQuery(const float pos[4],const float R,
		GLuint pixCountThreshold=0//渲染像素数<=pixCountThreshold认为遮挡
		)//返回是否遮挡（或在视野外）
	{
		glBeginQueryARB( GL_SAMPLES_PASSED_ARB, occQuery );
		renderBox(pos,R);
		glEndQueryARB( GL_SAMPLES_PASSED_ARB );
		//获得结果
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
inline bool isPointEq(const float p0[4],const float p1[4]){//判断两点是否重合
		const float localEps=0.0001;//已除以10
		if(fabs(p0[X]-p1[X])<localEps
			&&fabs(p0[Y]-p1[Y])<localEps
			&&fabs(p0[Z]-p1[Z])<localEps){
			return true;
		}else{
			return false;
		}
	}

inline	bool getNormalOfTri(const float p0[4],const float p1[4],const float p2[4],float norm[4]){//获得三角面的法向量，前端返回是否构成三角形
		float p01[4],p02[4];
		sub((float(&)[4])(*p1),(float(&)[4])(*p0),p01);
		sub((float(&)[4])(*p2),(float(&)[4])(*p0),p02);
		cross(p01,p02,norm);
		if(normalize(norm)==0){//如果nrom模为0，说明三点共线
			return false;
		}
		return true;
	}

inline bool isInSight_small(float c[4],float R,
	GLdouble modelMatrix[16],
	GLdouble projMatrix[16]){//判断以c为中心，边心距为R(R<<视椎)，边平行于坐标轴的立方体是否在视截体内
	GLint viewport[4]={0,0,widthEx,height};
	float x=c[X];
	float y=c[Y];
	float z=c[Z];
	//世界坐标系内立方体的八个顶点坐标
	float p[8][4];
	//下底面
	init4(p[0],x-R,y-R,z-R,1);
	init4(p[1],x-R,y-R,z+R,1);
	init4(p[2],x+R,y-R,z+R,1);
	init4(p[3],x+R,y-R,z-R,1);
	//上底面
	init4(p[4],x-R,y+R,z-R,1);
	init4(p[5],x-R,y+R,z+R,1);
	init4(p[6],x+R,y+R,z+R,1);
	init4(p[7],x+R,y+R,z-R,1);
	//将p投影到屏幕空间，看有没有落在视截体内者，由于R<<视锥，所以不可能出现立方体包含视锥的情况，所以当且仅当有顶点落在视锥内时与视锥相交
	//由于只判断顶点是否在视锥内，而不要求拓扑关系正确，所以无需对虚像进行校正
	for(int i=0;i<=7;i++){
		GLdouble winx,winy,winz;
		gluProject(p[i][X],p[i][Y],p[i][Z],modelMatrix,projMatrix,viewport,&winx,&winy,&winz);
		//判断(winx,winy,winz)是否在屏幕空间内
		if(winx>=viewport[0]&&winx<=viewport[0]+viewport[2]
			&&winy>=viewport[1]&&winy<=viewport[1]+viewport[3]
			&&winz>=0&&winz<=1){//在屏幕空间内
			return true;
		}
	}
	return false;
}

inline void makeShadowStencil_zpass(vector<Cv4>&vlist,float light_pos[4],float campos[4])//对轮廓vlist产生阴影模板
//其中vlist中的坐标均为世界坐标
//此函数的调用环境：
//“开启混合，开启深度测试，禁写z-buff，关闭纹理，关闭光照，关闭color_material，关闭雾”，即：
//	glEnable(GL_STENCIL_TEST);
//	glDepthMask(GL_FALSE);
//  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
//	glDisable(GL_TEXTURE_2D);
//  glDisable(GL_LIGHTING);
//  glDisable(GL_FOG);
{
	
	//以light_pos为中心对vlist进行放大
	vector<Cv4> vlist2=vlist;//vlist2为放大结果
	int n=vlist.size();
	for(int i=0;i<n;i++){
		float *v=vlist[i].v;
		float *v2=vlist2[i].v;
		sub((float(&)[4])(*v),(float(&)[4])(*light_pos),(float(&)[4])(*v2));//mark!
		mul(/*1000*/100,v2,v2);
		add(light_pos,v2,v2);
		v2[W]=1;
	}//得到vlist2

	glStencilFunc(GL_ALWAYS,1,1);

	//画正向面
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

	//画背向面
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
	glCullFace(GL_BACK);//恢复为系统默认值BACK

		

}
inline void renderStencilShadow(float alpha,float _width,float _height){//渲染模板阴影
	//获得视口参数
	GLint viewport[4]={0,0,_width,_height};
	//渲染视口
	glStencilFunc(GL_LESS,defaultStencilValue,~0);//defaultStencilValue小于模板值的部分测试通过进行渲染
	                                             
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


inline	float PND_point_plane(const Cplane&plane,const float point[4]){//是点到平面的有向距离
		const float *norm=plane.norm;
		const float *p=plane.p;
		float ppoint[4];
		sub((float(&)[4])(*point),(float(&)[4])(*p),ppoint);
		float PNd=dot(norm,ppoint);
		return PNd;
		
}
inline void calLinearModel(const float x0,const float y0,//数据1
						   const float x1,const float y1,//数据2
						   float&a,float&b//解出的系数
						   )
//求解线性模型
// 变量：     X        Y
//-----------------------------
// 数据：     x0       y0
//            x1       y1
//-----------------------------
// 模型： Y=aX+b
//-----------------------------
// 解得： a=(y1-y0)/(x1-x0)
//        b=(x1y0-x0y1)/(x1-x0)
//*********************??????????????????????这个函数的执行时间是0吗?????????
{
	a=(y1-y0)/(x1-x0);
	b=(x1*y0-x0*y1)/(x1-x0);
}
inline void readLandMat(){
	//读取高程数据
		const int MAKEUP=(4-(BMPWIDTH*3)%4)%4;
		//data和_data只是两个临时中转用的数组，如果申请静态数组，会使栈空间不够用，
		//所以这里改用申请动态空间
		BYTE *data=new BYTE[BMPHEIGHT*(BMPWIDTH*3+MAKEUP)];//位图数据缓冲
		BYTE *_data=new BYTE[BMPHEIGHT*BMPWIDTH];//提取数据的数组
		
	
			string pathName="data\\land\\land.bmp";
				//读取位图
				FILE  *infile=NULL;
				//打开文件
				errno_t eno=fopen_s(&infile,pathName.c_str(),"rb");
				if(eno!=0)
				{ 
					printf("Open 24 bit bitmap file failed!"); 
					return;
				}
					//读文件头
				BYTE  Head[54];
				fread(Head,54,1,infile); 
				//读数据
				
				
			int len=BMPHEIGHT*(BMPWIDTH*3+MAKEUP);
				fread(data,len,1,infile);
				fclose(infile);
					//分析文件头
				int bmpWidth =Head[18]+Head[19]*256;
				int bmpHeight=Head[22]+Head[23]*256;
				if(bmpWidth!=BMPWIDTH||bmpHeight!=BMPHEIGHT){
					cout<<"配置不匹配!"<<endl;
					exit(0);
					return ;
				}
				int makeup=(4-(bmpWidth*3)%4)%4;
				if(makeup!=MAKEUP){
					cout<<"配置不匹配!"<<endl;
					exit(0);
					return ;
				}
	
		//提取数据
				int p=0;
				int _p=0;
				for(int i=0;i<bmpHeight;i++){
					for(int j=0;j<bmpWidth;j++){
					
						_data[_p++]=data[p];//blue 因为bmp的通道顺序是bgr
						
						p+=3;
					}
					p+=MAKEUP;
				}
					//将_data填充到landMat并放缩
				_p=0;
				for(int i=BMPHEIGHT-1;i>=0;i--){//倒序
					for(int j=0;j<BMPWIDTH;j++){//正序
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
inline void softMov(const float originPos[4],//初始位置
	const float tgPos[4],//目标位置
	const float percentage,//移动百分比
	float rsPos[4]//结果位置
	)//转移动
{
	//将originPos移到tgPos，软跟随效果，不一次到位，只移动percentage的比例
	float vec[4];//从originPos到tgPos的向量
	sub((float(&)[4])(*tgPos),(float(&)[4])(*originPos),vec);
	float dvec[4];//实际移动向量
	mul(percentage,vec,dvec);//得到dvec
	//移动
	add(originPos,dvec,rsPos);
}



			
