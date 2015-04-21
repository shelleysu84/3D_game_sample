typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

inline bool initGame(){	
wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

wglSwapIntervalEXT(1);//打开垂直同步，限制帧率
//wglSwapIntervalEXT(0);//关闭垂直同步


	//-------------opengl版本信息---------------------------------------------
	//详见：http://www.opengl.org/wiki/GlGetString
	cout<<endl;
	const GLubyte* s_gl_vendor = glGetString(GL_VENDOR); 
	const GLubyte* s_gl_renderer = glGetString(GL_RENDERER);   
	const GLubyte* s_gl_version =glGetString(GL_VERSION);
	const GLubyte* s_glu_version= gluGetString(GLU_VERSION);
	cout<<"OpenGL实现厂商："<<s_gl_vendor<<endl;    
	cout<<"GPU/渲染器："<<s_gl_renderer<<endl;    
	cout<<"OpenGL实现版本号："<<s_gl_version<<endl;    
	cout<<"GLU版本："<<s_glu_version<<endl;   
	
	//----------------------------------------------------------------------------------------------------------------------
	srand(time(0));//随机数
	//------------------------------------------------------------------------------------------------------------------------
	glClearColor(0.0,0.0,0.0,0.0);	//设置清除颜色缓冲时所用的颜色
	glEnable(GL_DEPTH_TEST);
	glClearStencil(defaultStencilValue);//设置清除模板缓冲时所用的值
//	glEnable(GL_STENCIL_TEST);
	//-----------------------------------------------------------------------------------------------------------------------
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//----------暗示
	glHint(GL_FOG_HINT,GL_FASTEST);
	glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT,GL_FASTEST);
	glHint(GL_GENERATE_MIPMAP_HINT,GL_FASTEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST); 
	glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST); 
	glHint(GL_TEXTURE_COMPRESSION_HINT,GL_FASTEST);
	//----启用
	//vbo
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//实测发现GL_POINT_SMOOTH和GL_LINE_SMOOTH较影响效率，所以这里不将它们全局开启，
	//只在非用不可的时候临时开启
//	//glEnable(GL_POINT_SMOOTH);
//	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);//开启光照
	glEnable(GL_LIGHT0);//开启LIGHT0
	//-----------颜色材质
	glEnable(GL_COLOR_MATERIAL);//开启它可以使用颜色代替某些材质参数，提高效率
	                            //用颜色代替哪些材质参数用glColorMaterial进行设定
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);//GL_FRONT_AND_BACK和GL_AMBIENT_AND_DIFFUSE均为默认值

	//-------------------------------------------光照-------------------------------------------------------------------------- 
	//----选择明暗技术
	//可以用GL_FLAT或GL_SMOOTH
	glShadeModel(GL_SMOOTH);//为了使球体圆滑（目前只有球体用的是点法向量）
	//----设置材质
	//可以对不同的面GL_FRONT,GL_BACK,GL_FRONT设置如下项目：
	//GL_AMBIENT(环境颜色),GL_DIFFUSE(散射颜色),GL_AMBIENT_AND_DIFFUSE,GL_SPECULAR(镜面颜色),GL_SHININESS(镜面指数),
	//GL_EMISSION(自发光)
	//这里只对GL_SPECULAR和GL_SHININESS作了设置，其余用默认值
	GLfloat mat_ambient[]={1,1,1,1};
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);//环境光颜色
	GLfloat mat_diffuse[]={1,1,1,1};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);//散射颜色
	glColor4f(1,1,1,1);//颜色（等diffuse与color绑定，它就相当于设置diffuse）
	GLfloat mat_specular[]={0,0,0,1};
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);//镜面颜色
	GLfloat mat_shininess[]={0};
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);//镜面指数
	GLfloat mat_emission[]={0.15,0.15,0.15,1};
	glMaterialfv(GL_FRONT,GL_EMISSION,mat_emission);//自发光
	//----防止法线因一致缩放而造成的长度改变，比glEnable(GL_NORMALIZE)效率高
	glDisable(GL_NORMALIZE);
	glEnable(GL_RESCALE_NORMAL);//只有当法线向量所经历的缩放为一致缩放（即各个方向的缩放数量相同）
	                         //并且法线最初为单位向量时，此方法才能得到正确结果
	
	//----设置光源LIGHT0
	//共有8个光源LIGHT0~LIGHT7
	//设置项有GL_AMBIENT,GL_DIFFUSE,GL_SPECULAR,GL_POSITION,GL_SPOT_DIRECTION,GL_SPOT_EXPONENT,GL_SPOT_CUTOFF,
	//GL_CONSTANT_ATTENUATION,GL_LINEAR_ATTENUATION,GL_QUADRATIC_ATTENUATION
	//这里只对GL_DIFFUSE和GL_SPECULAR进行设置，其余用默认值
	GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);//漫反射
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);//镜面光
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);//环境光

	//----设置光照模型
	//包括GL_LIGHT_MODEL_AMBIENT,GL_LIGHT_MODEL_LOCAL_VIEWR,GL_LIGHT_MODEL_TWO_SIDE,GL_LIGHT_MODEL_COLOR_CONTROL
	//这里只对GL_LIGHT_MODEL_AMBIENT进行一下设置，其余用默认值
	GLfloat lmodel_ambient[]={0.5,0.5,0.5,1.0};
   	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
	glColor4f(1,1,1,1);
	//----设置雾
	glFogi(GL_FOG_MODE,  GL_EXP2);   
    glFogfv(GL_FOG_COLOR, normalFogColor);                
    glFogf(GL_FOG_DENSITY,normalFogDesity);//雾的浓度，用于指数模式中    
//	//注意，GL_FOG_START和GL_FOG_END只对线性雾有效，对GL_EXP和GL_EXP2模式是没用作用的
//	glFogf(GL_FOG_START, 150);// 雾气的开始位置
//  glFogf(GL_FOG_END, (rx+rz)/2*2);//雾气结束位置
	//----设置背面剔除
	//注意，cullFace是按顶点方向是顺时针还是逆时针来剔除的，而与提交的法向量方向无关
	glCullFace(GL_BACK);//隐藏背面，如果把参数改为GL_FRONT则隐藏正面。
	glDisable(GL_CULL_FACE);
	//------------------------纹理-----------------------------------
	glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);//glDepthFunc的默认值就是GL_LESS
	//---------------------------检查是否支持vbo扩展-----------------------------------------------------
	cout<<endl;
	cout<<"检查系统是否支持vbo..."<<endl;
	supportVBO = IsExtensionSupported( "GL_ARB_vertex_buffer_object" ); 
	cout<<"检查完毕!"<<endl;
//	supportVBO=false;
	if(supportVBO){
		cout<<"系统支持VBO!"<<endl;
	}else{
		cout<<"系统不支持VBO!"<<endl;
		MessageBox(NULL,_T("系统不支持VBO!"),
			_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		exit(0);//没必要处理各种析构
	}
    //---------------------------检查是否支持fbo扩展-----------------------------------------------------
	cout<<endl;
	cout<<"检查系统是否支持fbo..."<<endl;
	supportFBO = IsExtensionSupported( "GL_EXT_framebuffer_object" ); 
	cout<<"检查完毕!"<<endl;
//	supportFBO=false;
	if(supportFBO){
		cout<<"系统支持FBO!"<<endl;
	}else{
		cout<<"系统不支持FBO!"<<endl;
		MessageBox(NULL,_T("系统不支持FBO!"),
			_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		exit(0);//没必要处理各种析构
	}

	//---------------------------Get Pointers To The GL Functions (一定要放在初始化opengl之后，才能保证获得的函数指针不为空)
  
	if(supportVBO){
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB"); 
        glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB"); 
        glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB"); 
        glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB"); 
		glMapBufferARB = (PFNGLMAPBUFFERARBPROC) wglGetProcAddress("glMapBufferARB"); 
		glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC) wglGetProcAddress("glUnmapBufferARB"); 
 	}
    if(supportFBO){
        glGenFramebuffersEXT                     = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
        glDeleteFramebuffersEXT                  = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
        glBindFramebufferEXT                     = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
        glCheckFramebufferStatusEXT              = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
        glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
        glGenerateMipmapEXT                      = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");
        glFramebufferTexture2DEXT                = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
        glFramebufferRenderbufferEXT             = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
        glGenRenderbuffersEXT                    = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
        glDeleteRenderbuffersEXT                 = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
        glBindRenderbufferEXT                    = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
        glRenderbufferStorageEXT                 = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
        glGetRenderbufferParameterivEXT          = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
        glIsRenderbufferEXT                      = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
	}
		//-------------创建屏幕fbo
	if(supportFBO){
        screenFBO.createFBO(512*2,512,2,1,
			GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR,
			GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR,
			//GL_LINEAR,GL_LINEAR
			GL_NEAREST,GL_NEAREST
			);
    }
/*	//------------------------------------------遮挡检测
	char *ext = (char*)glGetString( GL_EXTENSIONS );

	if( strstr( ext, "GL_ARB_occlusion_query" ) == NULL )
	{
		MessageBox(NULL,_T("GL_ARB_occlusion_query extension was not found"),
			_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		exit(0);
		return false;
	}
	else
	{
		glGenQueriesARB = (PFNGLGENQUERIESARBPROC)wglGetProcAddress("glGenQueriesARB");
		glDeleteQueriesARB = (PFNGLDELETEQUERIESARBPROC)wglGetProcAddress("glDeleteQueriesARB");
		glIsQueryARB = (PFNGLISQUERYARBPROC)wglGetProcAddress("glIsQueryARB");
		glBeginQueryARB = (PFNGLBEGINQUERYARBPROC)wglGetProcAddress("glBeginQueryARB");
		glEndQueryARB = (PFNGLENDQUERYARBPROC)wglGetProcAddress("glEndQueryARB");
		glGetQueryivARB = (PFNGLGETQUERYIVARBPROC)wglGetProcAddress("glGetQueryivARB");
		glGetQueryObjectivARB = (PFNGLGETQUERYOBJECTIVARBPROC)wglGetProcAddress("glGetQueryObjectivARB");
		glGetQueryObjectuivARB = (PFNGLGETQUERYOBJECTUIVARBPROC)wglGetProcAddress("glGetQueryObjectuivARB");

		if( !glGenQueriesARB || !glDeleteQueriesARB || !glIsQueryARB || 
			!glBeginQueryARB || !glEndQueryARB || !glGetQueryivARB ||
			!glGetQueryObjectivARB || !glGetQueryObjectuivARB )
		{
			MessageBox(NULL,_T("One or more GL_ARB_occlusion_query functions were not found"),
				_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
			exit(0);
			return false;
		}
	}
	//--------------------------创建遮挡查询
	glGenQueriesARB( 1, &occQuery );
	*/
	//---------------------------------shader-------------------------------------
	//检查是否支持opengl 2.0
	cout<<endl;
	cout<<"检查系统是否支持openGL 2.0..."<<endl;
	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		cout<<"系统支持openGL 2.0!"<<endl;
	else {
		cout<<"系统不支持openGL 2.0!"<<endl;
		system("pause");
		exit(0);
	}
	//----------加载，编译shader，组装shader程序-------------------------------------------
	loadAndCompileShader(v,"data\\shader\\shader1.vert");
	loadAndCompileShader(f,"data\\shader\\shader1.frag");
	loadAndCompileShader(f2,"data\\shader\\shader2.frag");
	loadAndCompileShader(f3,"data\\shader\\shader3.frag");
	loadAndCompileShader(f4,"data\\shader\\shader4.frag");
	makeShaderProgram(p,v,f);
	makeShaderProgram(p2,v,f2);
	makeShaderProgram(p3,v,f3);
	makeShaderProgram(p4,v,f4);
	//------------------------------------------------------
	//------------------------加载全局纹理---------------------------------------
	//（每次添加新纹理前先看看是否有老的不用的纹理可以替换掉）
	tex_white=textureManager.CreateTexture("data\\white.bmp");
	if(tex_white==0)return false;
	tex_ground=textureManager.CreateTexture("data\\ground.bmp");
	if(tex_ground==0)return false;
	tex_water=textureManager.CreateTexture("data\\water.bmp");
	if(tex_water==0)return false;
	tex_reflection_off=textureManager.CreateTexture("data\\reflection_off.bmp");
	if(tex_reflection_off==0)return false;
	tex_unfinished=textureManager.CreateTexture("data\\unfinished.bmp",GL_LINEAR_MIPMAP_NEAREST,GL_LINEAR);
	if(tex_unfinished==0)return false;
	tex_finished=textureManager.CreateTexture("data\\finished.bmp",GL_LINEAR_MIPMAP_NEAREST,GL_LINEAR);
	if(tex_finished==0)return false;
	tex_protein2d=textureManager.CreateTexture("data\\protein2d.bmp",GL_LINEAR_MIPMAP_NEAREST,GL_LINEAR);
	if(tex_protein2d==0)return false;
	tex_molecuIcon=textureManager.CreateTexture("data\\molecuIcon.bmp",GL_LINEAR_MIPMAP_NEAREST,GL_LINEAR);
	if(tex_molecuIcon==0)return false;
	//-----------------------加载fbx模型---------------------------------------
	//加载fbx模型
	cout<<endl;
	modelx.modelfbx.Init_and_load("data\\model_fbx\\dinosaur\\dinosaur.fbx");
	modelx2.modelfbx.Init_and_load("data\\model_fbx\\dance1\\dance1.fbx");
	house_temp.modelfbx.Init_and_load("data\\model_fbx\\scene\\scene.fbx");
	gene_r_temp.modelfbx.Init_and_load("data\\model_fbx\\gene_r\\gene_r.fbx");
	gene_g_temp.modelfbx.Init_and_load("data\\model_fbx\\gene_g\\gene_g.fbx");
	gene_b_temp.modelfbx.Init_and_load("data\\model_fbx\\gene_b\\gene_b.fbx");
	gene_rgbrgb_temp.modelfbx.Init_and_load("data\\model_fbx\\gene_rgbrgb\\gene_rgbrgb.fbx");
	amac_r_temp.modelfbx.Init_and_load("data\\model_fbx\\amac_r\\amac_r.fbx");
	amac_g_temp.modelfbx.Init_and_load("data\\model_fbx\\amac_g\\amac_g.fbx");
	amac_b_temp.modelfbx.Init_and_load("data\\model_fbx\\amac_b\\amac_b.fbx");
	ene_r_temp.modelfbx.Init_and_load("data\\model_fbx\\ene_r\\ene_r.fbx");
	protein_temp.modelfbx.Init_and_load("data\\model_fbx\\protein\\protein.fbx");
	tree0_temp.modelfbx.Init_and_load("data\\model_fbx\\tree0\\tree0.fbx");
	tree1_temp.modelfbx.Init_and_load("data\\model_fbx\\tree1\\tree1.fbx");
	tree2_temp.modelfbx.Init_and_load("data\\model_fbx\\tree2\\tree2.fbx");
	bigcannon_temp.modelfbx.Init_and_load("data\\model_fbx\\bigcannon\\bigcannon.fbx");
	skyball_temp.modelfbx.Init_and_load("data\\model_fbx\\skyball\\skyball.fbx");
	cell_temp.modelfbx.Init_and_load("data\\model_fbx\\cell\\cell.fbx");
	cell2_temp.modelfbx.Init_and_load("data\\model_fbx\\cell2\\cell2.fbx");
	bubble_temp.modelfbx.Init_and_load("data\\model_fbx\\bubble\\bubble.fbx");
	drop_temp.modelfbx.Init_and_load("data\\model_fbx\\drop\\drop.fbx");
	cannonball_temp.modelfbx.Init_and_load("data\\model_fbx\\cannonball\\cannonball.fbx");
	//加载模型纹理
	modelx.modelfbx.triangulate_loadTextures_preprocess();
	modelx2.modelfbx.triangulate_loadTextures_preprocess();
	house_temp.modelfbx.triangulate_loadTextures_preprocess();
	gene_r_temp.modelfbx.triangulate_loadTextures_preprocess();
	gene_g_temp.modelfbx.triangulate_loadTextures_preprocess();
	gene_b_temp.modelfbx.triangulate_loadTextures_preprocess();
	gene_rgbrgb_temp.modelfbx.triangulate_loadTextures_preprocess();
	amac_r_temp.modelfbx.triangulate_loadTextures_preprocess();
	amac_g_temp.modelfbx.triangulate_loadTextures_preprocess();
	amac_b_temp.modelfbx.triangulate_loadTextures_preprocess();
	ene_r_temp.modelfbx.triangulate_loadTextures_preprocess();
	protein_temp.modelfbx.triangulate_loadTextures_preprocess();
	tree0_temp.modelfbx.triangulate_loadTextures_preprocess();
	tree1_temp.modelfbx.triangulate_loadTextures_preprocess();
	tree2_temp.modelfbx.triangulate_loadTextures_preprocess();
	bigcannon_temp.modelfbx.triangulate_loadTextures_preprocess();
	skyball_temp.modelfbx.triangulate_loadTextures_preprocess();
	cell_temp.modelfbx.triangulate_loadTextures_preprocess();
	cell2_temp.modelfbx.triangulate_loadTextures_preprocess();
	bubble_temp.modelfbx.triangulate_loadTextures_preprocess();
	drop_temp.modelfbx.triangulate_loadTextures_preprocess();
	cannonball_temp.modelfbx.triangulate_loadTextures_preprocess();
	//设置动画
	modelx.modelfbx.setTime(0);
	modelx.modelfbx.selectAnimation(0);
	modelx2.modelfbx.setTime(0);
	modelx2.modelfbx.selectAnimation(0);
	house_temp.modelfbx.setTime(0);
	house_temp.modelfbx.selectAnimation(0);
	gene_r_temp.modelfbx.setTime(0);
	gene_r_temp.modelfbx.selectAnimation(0);
	gene_g_temp.modelfbx.setTime(0);
	gene_g_temp.modelfbx.selectAnimation(0);
	gene_b_temp.modelfbx.setTime(0);
	gene_b_temp.modelfbx.selectAnimation(0);
	gene_rgbrgb_temp.modelfbx.setTime(0);
	gene_rgbrgb_temp.modelfbx.selectAnimation(0);
	amac_r_temp.modelfbx.setTime(0);
	amac_r_temp.modelfbx.selectAnimation(0);
	amac_g_temp.modelfbx.setTime(0);
	amac_g_temp.modelfbx.selectAnimation(0);
	amac_b_temp.modelfbx.setTime(0);
	amac_b_temp.modelfbx.selectAnimation(0);
	ene_r_temp.modelfbx.setTime(0);
	ene_r_temp.modelfbx.selectAnimation(0);
	protein_temp.modelfbx.setTime(0);
	protein_temp.modelfbx.selectAnimation(0);
	tree0_temp.modelfbx.setTime(0);
	tree0_temp.modelfbx.selectAnimation(0);
	tree1_temp.modelfbx.setTime(0);
	tree1_temp.modelfbx.selectAnimation(0);
	tree2_temp.modelfbx.setTime(0);
	tree2_temp.modelfbx.selectAnimation(0);
	bigcannon_temp.modelfbx.setTime(0);
	bigcannon_temp.modelfbx.selectAnimation(0);
	skyball_temp.modelfbx.setTime(0);
	skyball_temp.modelfbx.selectAnimation(0);
	cell_temp.modelfbx.setTime(0);
	cell_temp.modelfbx.selectAnimation(0);
	cell2_temp.modelfbx.setTime(0);
	cell2_temp.modelfbx.selectAnimation(0);
	bubble_temp.modelfbx.setTime(0);
	bubble_temp.modelfbx.selectAnimation(0);
	drop_temp.modelfbx.setTime(0);
	drop_temp.modelfbx.selectAnimation(0);
	cannonball_temp.modelfbx.setTime(0);
	cannonball_temp.modelfbx.selectAnimation(0);
	//---------------------------------生成字符串显示列表------------------------------------------------------------------------
	//将NproteinMax转化成字符串
	{
		NproteinMax_str;
		char numbuff[20];//数值字串缓冲区
		_itoa_s(NproteinMax,numbuff,10);
		NproteinMax_str=numbuff;
	}
	//清空者为不使用，但为了占位不能删
	CreateCNTextDispList("生命:");//0
	CreateCNTextDispList("分数:");//1
	CreateCNTextDispList("任务未完成...");//2
	CreateCNTextDispList("任务已完成,yeah!");//3
	CreateCNTextDispList("");//4
	CreateCNTextDispList("技能:");//5
	CreateCNTextDispList("跳跃");//6
	CreateCNTextDispList("无敌");//7
	CreateCNTextDispList("加速");//8
	CreateCNTextDispList("弹路:");//9
	CreateCNTextDispList("");//10
	CreateCNTextDispList("暂停");//11
	CreateCNTextDispList("游戏结束!");//12
	CreateCNTextDispList("按C键继续,分数将丢失");//13
	CreateCNTextDispList("可由'菜单->作弊'关闭作弊");//14
	CreateCNTextDispList("可由'菜单->作弊'开所有技能");//15
	CreateCNTextDispList("作弊：已开启");//16
	CreateCNTextDispList("作弊：已关闭");//17
	string tstr="任务：收集";tstr+=NproteinMax_str;tstr+="个超级基因";
	CreateCNTextDispList(tstr.c_str());//18
	//---------------------------------读取地形数据----------------------------------------------------------------------------
	cout<<endl;
	cout<<"读取地形数据..."<<endl;
	readLandMat();
	cout<<"读取完毕!"<<endl;
	
	//---------------------------------获得视口大小--------------------------------------------------------------------------
	//获得视口大小
	GLint viewport[4];//4个元素依次为viewX,viewY,width,height
	glGetIntegerv(GL_VIEWPORT,viewport);
	widthEx=viewport[2];
	height=viewport[3];

	//---------------------------------俯视范围长宽及地形块长宽--------------------------------------------------------------
	areaHeight_god=331.371;//god俯视范围长宽(世界尺寸)
	areaWidth_god=331.371;
	blockWidth=areaWidth_god*8.67;//4;//只要blockWidth>=areaWidth_god即可
	blockHeight=areaHeight_god*8.67;//4;
	//活动范围长宽(活动范围以pcell为中心)
	width_mov=areaHeight_god*3;
	height_mov=areaWidth_god*3;
	//物体LOD参考距离
	fakeDisMax=(width_mov+height_mov)/2;
	fakeDisMaxDiv6=fakeDisMax/6;
	fakeDisMaxDiv3=fakeDisMax/3;

	
	//---------------------------------建立左视口ground场景树------------------------------------------------------------
	//----制作ground
	ground.init(blockWidth,blockHeight,-blockWidth/2,-blockHeight/2);
	ground.texture=tex_ground;
//	ground.visible=false;
	ground.setKuKv(0.005,0.005);
	ground.linkFrustum(&frustum);
	ground.fillNormalMat(landMat,normalMat);//生成地形法向数据(必须放在设定好地形块大小之后)
	//----制作*pcell
	pcell=new CmyCell();
	cellRoot.addchild(pcell);
	pcell->setGround(&ground);
	pcell->setWater(&water);
	pcell->setV(pcell->vlen_slow,0,0);
	pcell->setRc(Rc_cell);
	init4(pcell->flglrcolor,1,70.0/255,70.0/255,1);
	pcell->bshell=true;
	pcell->bflglr=true;
	pcell->movloc((31366-32847)/10,2712.58/10+500,(13638.1-13080)/10);//定位pcell
	pcell->rotloc010(60+90+90);//调整pcell方向
	pcell->setjumpable(true);
//	pcell->bshowBB=true;
	pcell->setNcannonball(40);
	{
		Cmesh*pmesh=NULL;
		cannonball_temp.modelfbx.show_ToMesh(pmesh);
		pmesh->calTextureInfo();
		pmesh->directVBO_singleTex.makeVBO(pmesh->vlist,pmesh->IDtriList,GL_TRIANGLES);
		pcell->createCannonballs(pmesh);
	}
	cell_temp.modelfbx.show_ToMeshList(pcell->pmeshList);
	for(int i=0;i<(int)pcell->pmeshList.size();i++){
		pcell->pmeshList[i]->calTextureInfo();
		pcell->pmeshList[i]->directVBO_singleTex.makeVBO(pcell->pmeshList[i]->vlist,pcell->pmeshList[i]->IDtriList,GL_TRIANGLES);
	}
	//获得pcell位置
	float cellpos[4];
	pcell->getPosInFather(cellpos);
	//设定相机位置
	{
		float campos_incell[4]={0,320,600,1};//细胞坐标系下相机的位置
		pcell->toFather_wOne(campos_incell,frustum.campos);//得到frustum.campos
	}
	//----制作house
/*	//在house_temp.modelfbx中读取唯一的mesh
	{
		Cmesh*pmesh=NULL;
		house_temp.modelfbx.show_ToMesh(pmesh);
		pmesh->calTextureInfo();
		pmesh->indexVBO.makeVBO(pmesh->vlist,pmesh->IDtriList,pmesh->textureList,pmesh->texIDtriCountList,GL_TRIANGLES);
		house.pmesh=pmesh;
	}*/
	//在house_temp.modelfbx中读取所有mesh，并处理属性后再合并
	{
		vector<Cmesh*> pmeshList;//mesh列表
		house_temp.modelfbx.show_ToMeshList(pmeshList);//得到pmeshList
		//将pmeshList中各mesh合并成一个mesh（都合并到pmeshList[0]）
		{
			int nMesh=(int)pmeshList.size();
			for(int i=1;i<nMesh;i++){//注意是从1开始的
				Cmesh*pmesh=pmeshList[i];
				pmeshList[0]->merge(*pmesh);
			}
		}
		//删除多余的mesh（pmeshList[1~nMesh-1]删除）
		{
			int nMesh=(int)pmeshList.size();
			for(int i=1;i<nMesh;i++){//注意是从1开始的
				Cmesh*pmesh=pmeshList[i];
				delete pmesh;
			}
		}
		//将唯一的mesh赋给house.pmesh
		house.pmesh=pmeshList[0];
		//处理house.pmesh
		house.pmesh->calTextureInfo();
		house.pmesh->indexVBO.makeVBO(house.pmesh->vlist,house.pmesh->IDtriList,house.pmesh->textureList,house.pmesh->texIDtriCountList,GL_TRIANGLES);
	}
	house.makeOctree();
	house.setPcell(pcell);
	house.linkFrustum(&frustum);
	//----制作metalBall
	metalBall.movloc(209,25+60,-53);
	metalBall.setRc(20.0);
//	metalBall.bshowBB=true;
	metalBall.reflection_off=metalBallReflectionOff;
	if(supportFBO){
        metalBall.fbo.createFBO(256,256,1,1,
			GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR,
			GL_NEAREST_MIPMAP_NEAREST,GL_NEAREST,
			GL_NEAREST,GL_NEAREST);
    }
	{//为金属球制作显示列表
		GLuint displist=displistManager.genDisplist();
		glNewList(displist,GL_COMPILE);
		ball_useNormalAsUV(13,20.0);
		glEndList();
		//赋给metalBall
		metalBall.displist=displist;
	}
	//----制作天空盒
	float rx,rz,ry;
	rx=areaWidth_god*6*2.3;
	rz=areaHeight_god*6*2.3;
	ry=(rx+rz)/2;
    skyball.zoomloc(rx,ry,rz);
	skyball_temp.modelfbx.show_ToMesh(skyball.pmesh);
	skyball.pmesh->calTextureInfo();
	skyball.pmesh->directVBO_singleTex.makeVBO(skyball.pmesh->vlist,skyball.pmesh->IDtriList,GL_TRIANGLES);
	//----制作water
	water.init(Rc_cell*6,Rc_cell*6);
	water.texture=tex_water;
	water.setHorizon(waterHorizon);
	//----制作axis
	ground.addchild(&axis);
	axis.setLenXYZ(1000,1000,1000);
	axis.movloc(0,50,0);
	axis.visible=false;
	//----制作pproteinList
	{
		//制作网格样品列表
		const int nSample=1;
		Cmesh*pmeshSampleList_protein[nSample];//网格样品列表
		protein_temp.modelfbx.show_ToMesh(pmeshSampleList_protein[0]);
		pmeshSampleList_protein[0]->calTextureInfo();
		pmeshSampleList_protein[0]->directVBO_singleTex.makeVBO(pmeshSampleList_protein[0]->vlist,pmeshSampleList_protein[0]->IDtriList,GL_TRIANGLES);
		//制作proteinList
		float proteinPosList[NproteinMax][4]={//protein位置列表
			{(34317.3-32847)/10+20  ,(4691.76+200)/10+50,(20961.5-13080)/10-185,1},//塔尖
			{244,294+45,69,1},//火箭
			{(25957.9-32847)/10+20+90  ,(5336.5)/10+20,(20823.2-13080)/10-180,1},//空中台阶
			{(31204.5-32847)/10+20  ,(2981.88+350+200)/10,(5790.72-13080)/10+10,1},//隧道
			{255,1662+40,-1967,1},//空中小屋
			{-296,2275+45,-1642,1}//茶壶顶
		};
		pproteinList.reserve(NproteinMax);//预留
		for(int i=0;i<NproteinMax;i++){
			Cprotein*pprotein=new Cprotein();
			proteinRoot.addchild(pprotein);
			pprotein->movloc(proteinPosList[i][X],proteinPosList[i][Y],proteinPosList[i][Z]);
			pprotein->zoomloc(3,3,3);
			//	protein.bshowBB=true;
			pprotein->Rc=10.0;
			pproteinList.push_back(pprotein);
			int sampleIndex=rand()%nSample;
			pprotein->pmesh=pmeshSampleList_protein[sampleIndex];
		}
	}
	
	//----制作modelx
	modelx.setGround(&ground);
	modelx.modelfbx.rotFbxScene_general(1,0,0,-90);
	modelx.modelfbx.rotFbxScene_general(0,0,1,180);
	modelx.modelfbx.movFbxScene(0,0,-110);//向下移动一些（经过前面旋转，现在+Z为向上），这个移动量是放缩前的，所以应较大
	modelx.movloc((22915-32847)/10+20  ,306/10,(12383-13080)/10-135);
	modelx.rotloc010(-90-60+45);
	modelx.HaboveGround=11.5;
	modelx.setRc(13.0);
//	modelx.bshowBB=true;
	modelx.zoomloc(0.1,0.1,0.1);//加的
	fbxModelRoot.addchild(&modelx);

	//----制作modelx2
	modelx2.setGround(&ground);
	modelx2.modelfbx.movFbxScene(0,0,-4.0);//向下移动一些（经过前面旋转，现在+Z为向上）
	modelx2.movloc((32591-20-32847)/10+50,(820+130-160)/10,(10357.1-30-13080)/10-52.5);
	modelx2.setRc(18.5);
//	modelx2.bshowBB=true;
	modelx2.zoomloc(0.43,0.43,0.43);
	fbxModelRoot.addchild(&modelx2);

	//----生成pdiedsuperburstList
	{
		psuperburstList.reserve(Nsuperburst);
		pdiedsuperburstList.reserve(Nsuperburst);
		for(int i=0;i<Nsuperburst;i++){
			Csuperburst*psuperburst=new Csuperburst();
			psuperburst->init();
			psuperburst->setRc(Rc_superburst);
		//	psuperburst->bshowBB=true;
			psuperburst->setDied(true);
			psuperburst->sourceMake(3);
			pdiedsuperburstList.push_back(psuperburst);



		}

	}
	//----制作pdiedautoCellList列表
	{
		vector<Cmesh*> pmeshList;
		cell2_temp.modelfbx.show_ToMeshList(pmeshList);
		for(int i=0;i<(int)pmeshList.size();i++){
			pmeshList[i]->calTextureInfo();
			pmeshList[i]->directVBO_singleTex.makeVBO(pmeshList[i]->vlist,pmeshList[i]->IDtriList,GL_TRIANGLES);
		}
		pautoCellList.reserve(NautoCell);
		pdiedautoCellList.reserve(NautoCell);
		for(int i=0;i<NautoCell;i++){
			CautoCell*pautoCell=new CautoCell();
			pautoCell->setGround(&ground);
			pautoCell->setWater(&water);
			init4(pautoCell->flglrcolor,70.0/255,1,70.0/255,1);
			{
				for(int j=0;j<(int)pmeshList.size();j++){
					pautoCell->pmeshList.push_back(pmeshList[j]);
				}
			}
			pautoCell->setRc(Rc_autoCell);
			//		autoCell.bshowBB=true;
			pautoCell->bmoving=true;
			pautoCell->sourceMake(13);
			//只加入到死亡敌菌列表
			pdiedautoCellList.push_back(pautoCell);
		}
	}
	
	//----制作tree
	{
		//制作网格样品列表
		const int nSample=3;
		Cmesh*pmeshSampleList_tree[nSample];//网格样品列表
		tree0_temp.modelfbx.show_ToMesh(pmeshSampleList_tree[0]);
		tree1_temp.modelfbx.show_ToMesh(pmeshSampleList_tree[1]);
		tree2_temp.modelfbx.show_ToMesh(pmeshSampleList_tree[2]);
		pmeshSampleList_tree[0]->calTextureInfo();
		pmeshSampleList_tree[1]->calTextureInfo();
		pmeshSampleList_tree[2]->calTextureInfo();
		pmeshSampleList_tree[0]->directVBO_singleTex.makeVBO(pmeshSampleList_tree[0]->vlist,pmeshSampleList_tree[0]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_tree[1]->directVBO_singleTex.makeVBO(pmeshSampleList_tree[1]->vlist,pmeshSampleList_tree[1]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_tree[2]->directVBO_singleTex.makeVBO(pmeshSampleList_tree[2]->vlist,pmeshSampleList_tree[2]->IDtriList,GL_TRIANGLES);
		//制作ptreeList
		ptreeList.reserve(Ntree);
		for(int i=0;i<Ntree;i++){
			Ctree*p=new Ctree();
			ptreeList.push_back(p);
			treeRoot.addchild(p);
			p->setGround(&ground);
			p->setWater(&water);
			int x=cellpos[X]+rand()%(int)(width_mov)-(width_mov)/2;
			int z=cellpos[Z]+rand()%(int)(height_mov)-(height_mov)/2;
			p->movloc(x,0,z);
			p->setRc(Rc_tree);
			//		p->bshowBB=true;
			//着陆
			float treenewpos[4];
			p->getPosInFather(treenewpos);
			float groundHorizon=ground.getH(treenewpos[X],treenewpos[Z]);
			if(groundHorizon>water.Horizon){
				p->downToGround(0,false);
			}else{
				p->downToWaterSurface();

			}
			int sampleIndex=rand()%nSample;
			p->pmesh=pmeshSampleList_tree[sampleIndex];

		}
	}
	//----制作pdiedamacList
	{
		//制作网格样品列表
		const int nSample=3;
		Cmesh*pmeshSampleList_amac[nSample];//网格样品列表
		amac_r_temp.modelfbx.show_ToMesh(pmeshSampleList_amac[0]);
		amac_g_temp.modelfbx.show_ToMesh(pmeshSampleList_amac[1]);
		amac_b_temp.modelfbx.show_ToMesh(pmeshSampleList_amac[2]);
		pmeshSampleList_amac[0]->calTextureInfo();
		pmeshSampleList_amac[1]->calTextureInfo();
		pmeshSampleList_amac[2]->calTextureInfo();
		pmeshSampleList_amac[0]->directVBO_singleTex.makeVBO(pmeshSampleList_amac[0]->vlist,pmeshSampleList_amac[0]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_amac[1]->directVBO_singleTex.makeVBO(pmeshSampleList_amac[1]->vlist,pmeshSampleList_amac[1]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_amac[2]->directVBO_singleTex.makeVBO(pmeshSampleList_amac[2]->vlist,pmeshSampleList_amac[2]->IDtriList,GL_TRIANGLES);
		//制作pdiedamacList
		pamacList.reserve(Namac);
		pdiedamacList.reserve(Namac);
		for(int i=0;i<Namac;i++){
			Camac*pamac=new Camac();
			pdiedamacList.push_back(pamac);
			Camac&amac=*pamac;
			amac.zoomloc(3,3,3);
			amac.setGround(&ground);
			amac.setWater(&water);
			//   	amac.bshowBB=true;
			amac.setRc(Rc_amac);
			int sampleIndex=rand()%nSample;
			amac.pmesh=pmeshSampleList_amac[sampleIndex];
		}
	}
	//----制作pdiedeneList
	{
		//制作网格样品列表
		const int nSample=1;
		Cmesh*pmeshSampleList_ene[nSample];//网格样品列表
		ene_r_temp.modelfbx.show_ToMesh(pmeshSampleList_ene[0]);
		pmeshSampleList_ene[0]->calTextureInfo();
		pmeshSampleList_ene[0]->directVBO_singleTex.makeVBO(pmeshSampleList_ene[0]->vlist,pmeshSampleList_ene[0]->IDtriList,GL_TRIANGLES);
		//制作peneList
		peneList.reserve(Nene);
		pdiedeneList.reserve(Nene);
		for(int i=0;i<Nene;i++){
			Cene*pene=new Cene();
			pdiedeneList.push_back(pene);
			Cene&ene=*pene;
			ene.zoomloc(3,3,3);
			ene.setGround(&ground);
			ene.setWater(&water);
			//	ene.bshowBB=true;
			ene.setRc(Rc_ene);
			int sampleIndex=rand()%nSample;
			ene.pmesh=pmeshSampleList_ene[sampleIndex];
		}
	}
	//----制作pdiedgeneList
	{
		//制作网格样品列表
		const int nSample=4;
		Cmesh*pmeshSampleList_gene[nSample];//网格样品列表
		gene_r_temp.modelfbx.show_ToMesh(pmeshSampleList_gene[0]);
		gene_g_temp.modelfbx.show_ToMesh(pmeshSampleList_gene[1]);
		gene_b_temp.modelfbx.show_ToMesh(pmeshSampleList_gene[2]);
		gene_rgbrgb_temp.modelfbx.show_ToMesh(pmeshSampleList_gene[3]);
		pmeshSampleList_gene[0]->calTextureInfo();
		pmeshSampleList_gene[1]->calTextureInfo();
		pmeshSampleList_gene[2]->calTextureInfo();
		pmeshSampleList_gene[3]->calTextureInfo();
		pmeshSampleList_gene[0]->directVBO_singleTex.makeVBO(pmeshSampleList_gene[0]->vlist,pmeshSampleList_gene[0]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_gene[1]->directVBO_singleTex.makeVBO(pmeshSampleList_gene[1]->vlist,pmeshSampleList_gene[1]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_gene[2]->directVBO_singleTex.makeVBO(pmeshSampleList_gene[2]->vlist,pmeshSampleList_gene[2]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_gene[3]->directVBO_singleTex.makeVBO(pmeshSampleList_gene[3]->vlist,pmeshSampleList_gene[3]->IDtriList,GL_TRIANGLES);
		//制作pdiedgeneList
		pgeneList.reserve(Ngene);
		pdiedgeneList.reserve(Ngene);
		for(int i=0;i<Ngene;i++){
			Cgene*pgene=new Cgene();
			pdiedgeneList.push_back(pgene);
			Cgene&gene=*pgene;
			gene.zoomloc(3,3,3);
			gene.setGround(&ground);
			gene.setWater(&water);
			//		gene.bshowBB=true;
			gene.setRc(Rc_gene);
			int sampleIndex=rand()%nSample;
			gene.pmesh=pmeshSampleList_gene[sampleIndex];
			gene.effectID=sampleIndex;
			
		}
	}
	//----制作dropColorList
	{
		Crgba rgba;
		rgba.init(1,0,0);
		dropColorList.push_back(rgba);
		rgba.init(0,1,0);
		dropColorList.push_back(rgba);
		rgba.init(0,0,1);
		dropColorList.push_back(rgba);

	}
	//----制作pdieddropList
	{
		Cmesh*pmesh=NULL;
		drop_temp.modelfbx.show_ToMesh(pmesh);
		pmesh->calTextureInfo();
		pmesh->directVBO_singleTex.makeVBO(pmesh->vlist,pmesh->IDtriList,GL_TRIANGLES);
		pdropList.reserve(Ndrop);
		pdieddropList.reserve(Ndrop);
		for(int i=0;i<Ndrop;i++){
			Cdrop*pdrop=new Cdrop();
			pdrop->pmesh=pmesh;
			pdieddropList.push_back(pdrop);
		}
	}
	//----制作pdiedbubbleList
	{
		Cmesh*pmesh=NULL;
		bubble_temp.modelfbx.show_ToMesh(pmesh);
		pmesh->calTextureInfo();
		pmesh->directVBO_singleTex.makeVBO(pmesh->vlist,pmesh->IDtriList,GL_TRIANGLES);
		pbubbleList.reserve(Nbubble);
		pdiedbubbleList.reserve(Nbubble);
		for(int i=0;i<Nbubble;i++){
			Cbubble*pbubble=new Cbubble();
			pdiedbubbleList.push_back(pbubble);
			pbubble->sourceMake(4);
			pbubble->sourceGain(0.2);
			pbubble->pmesh=pmesh;
		}
	}
	//----制作pdiedburstList
	{
		pburstList.reserve(Nburst);
		pdiedburstList.reserve(Nburst);
		for(int i=0;i<Nburst;i++){
			Cburst*pburst=new Cburst();
			pburst->sourceMake(9);
			pdiedburstList.push_back(pburst);
			
		}
	}
	//----pdiedbigcannonList
	{
		Cmesh* pmesh=NULL;
		bigcannon_temp.modelfbx.show_ToMesh(pmesh);
		pmesh->calTextureInfo();
		pmesh->directVBO_singleTex.makeVBO(pmesh->vlist,pmesh->IDtriList,GL_TRIANGLES);
		
		pbigcannonList.reserve(Nbigcannon);
		pdiedbigcannonList.reserve(Nbigcannon);
		for(int i=0;i<Nbigcannon;i++){
			Cbigcannon*pbigcannon=new Cbigcannon();
			pdiedbigcannonList.push_back(pbigcannon);
			pbigcannon->sourceMake(2);//建立声源
			pbigcannon->setRc(Rc_bigcannon);
			//	pbigcannon->bshowBB=true;
			pbigcannon->pmesh=pmesh;
		}
	}
	//--------------------------------------初始化碰撞网格框架--------------------------------------
	{
		//初始化autoCell的碰撞网格框架
		autoCellCgrid.setFrame(width_mov,height_mov,max(Rc_autoCell,Rc_cell)*2,max(Rc_autoCell,Rc_cell)*2);
		autoCellCgrid.reserve_clist(NautoCell);
		//初始化amac的碰撞网格框架
		amacCgrid.setFrame(width_mov,height_mov,max(Rc_cell,Rc_amac)*2,max(Rc_cell,Rc_amac)*2);
		amacCgrid.reserve_clist(Namac);
		//初始化ene的碰撞网格框架
		eneCgrid.setFrame(width_mov,height_mov,max(Rc_cell,Rc_ene)*2,max(Rc_cell,Rc_ene)*2);
		eneCgrid.reserve_clist(Nene);
		//初始化gene的碰撞网格框架
		geneCgrid.setFrame(width_mov,height_mov,max(Rc_cell,Rc_gene)*2,max(Rc_cell,Rc_gene)*2);
		geneCgrid.reserve_clist(Ngene);
		//初始化tree的碰撞网格框架
		treeCgrid.setFrame(width_mov,height_mov,max(Rc_cell,Rc_tree)*2,max(Rc_cell,Rc_tree)*2);
		treeCgrid.reserve_clist(Ntree);
	}
	//--------------------各根节点孩子容量reserve----------------------------------
	//fbxModelRoot.p.reserve();
	//proteinRoot.p.reserve();
	treeRoot.p.reserve(Ntree);
	superburstRoot.p.reserve(Nsuperburst);
	bigcannonRoot.p.reserve(Nbigcannon);
	burstRoot.p.reserve(Nburst);
	dropRoot.p.reserve(Ndrop);
	bubbleRoot.p.reserve(Nbubble);
	molecuRoot.p.reserve(Namac+Nene+Ngene);
	cellRoot.p.reserve(NautoCell+1);//1代表pcell
	//--------------------删除所有_temp模型
	house_temp.modelfbx.destroyAllUserData_and_manager();
	gene_r_temp.modelfbx.destroyAllUserData_and_manager();
	gene_g_temp.modelfbx.destroyAllUserData_and_manager();
	gene_b_temp.modelfbx.destroyAllUserData_and_manager();
	gene_rgbrgb_temp.modelfbx.destroyAllUserData_and_manager();
	amac_r_temp.modelfbx.destroyAllUserData_and_manager();
	amac_g_temp.modelfbx.destroyAllUserData_and_manager();
	amac_b_temp.modelfbx.destroyAllUserData_and_manager();
	ene_r_temp.modelfbx.destroyAllUserData_and_manager();
	protein_temp.modelfbx.destroyAllUserData_and_manager();
	tree0_temp.modelfbx.destroyAllUserData_and_manager();
	tree1_temp.modelfbx.destroyAllUserData_and_manager();
	tree2_temp.modelfbx.destroyAllUserData_and_manager();
	bigcannon_temp.modelfbx.destroyAllUserData_and_manager();
	skyball_temp.modelfbx.destroyAllUserData_and_manager();
	cell_temp.modelfbx.destroyAllUserData_and_manager();
	cell2_temp.modelfbx.destroyAllUserData_and_manager();
	bubble_temp.modelfbx.destroyAllUserData_and_manager();
	drop_temp.modelfbx.destroyAllUserData_and_manager();
	cannonball_temp.modelfbx.destroyAllUserData_and_manager();
	//--------------提高进程优先级-----------------------------------------------------------------
	
	SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);
	//-------------------------------------------------------------------------------------------
	return true;

}