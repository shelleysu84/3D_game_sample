typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

inline bool initGame(){	
wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

wglSwapIntervalEXT(1);//�򿪴�ֱͬ��������֡��
//wglSwapIntervalEXT(0);//�رմ�ֱͬ��


	//-------------opengl�汾��Ϣ---------------------------------------------
	//�����http://www.opengl.org/wiki/GlGetString
	cout<<endl;
	const GLubyte* s_gl_vendor = glGetString(GL_VENDOR); 
	const GLubyte* s_gl_renderer = glGetString(GL_RENDERER);   
	const GLubyte* s_gl_version =glGetString(GL_VERSION);
	const GLubyte* s_glu_version= gluGetString(GLU_VERSION);
	cout<<"OpenGLʵ�ֳ��̣�"<<s_gl_vendor<<endl;    
	cout<<"GPU/��Ⱦ����"<<s_gl_renderer<<endl;    
	cout<<"OpenGLʵ�ְ汾�ţ�"<<s_gl_version<<endl;    
	cout<<"GLU�汾��"<<s_glu_version<<endl;   
	
	//----------------------------------------------------------------------------------------------------------------------
	srand(time(0));//�����
	//------------------------------------------------------------------------------------------------------------------------
	glClearColor(0.0,0.0,0.0,0.0);	//���������ɫ����ʱ���õ���ɫ
	glEnable(GL_DEPTH_TEST);
	glClearStencil(defaultStencilValue);//�������ģ�建��ʱ���õ�ֵ
//	glEnable(GL_STENCIL_TEST);
	//-----------------------------------------------------------------------------------------------------------------------
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//----------��ʾ
	glHint(GL_FOG_HINT,GL_FASTEST);
	glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT,GL_FASTEST);
	glHint(GL_GENERATE_MIPMAP_HINT,GL_FASTEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST); 
	glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST); 
	glHint(GL_TEXTURE_COMPRESSION_HINT,GL_FASTEST);
	//----����
	//vbo
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//ʵ�ⷢ��GL_POINT_SMOOTH��GL_LINE_SMOOTH��Ӱ��Ч�ʣ��������ﲻ������ȫ�ֿ�����
	//ֻ�ڷ��ò��ɵ�ʱ����ʱ����
//	//glEnable(GL_POINT_SMOOTH);
//	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);//��������
	glEnable(GL_LIGHT0);//����LIGHT0
	//-----------��ɫ����
	glEnable(GL_COLOR_MATERIAL);//����������ʹ����ɫ����ĳЩ���ʲ��������Ч��
	                            //����ɫ������Щ���ʲ�����glColorMaterial�����趨
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);//GL_FRONT_AND_BACK��GL_AMBIENT_AND_DIFFUSE��ΪĬ��ֵ

	//-------------------------------------------����-------------------------------------------------------------------------- 
	//----ѡ����������
	//������GL_FLAT��GL_SMOOTH
	glShadeModel(GL_SMOOTH);//Ϊ��ʹ����Բ����Ŀǰֻ�������õ��ǵ㷨������
	//----���ò���
	//���ԶԲ�ͬ����GL_FRONT,GL_BACK,GL_FRONT����������Ŀ��
	//GL_AMBIENT(������ɫ),GL_DIFFUSE(ɢ����ɫ),GL_AMBIENT_AND_DIFFUSE,GL_SPECULAR(������ɫ),GL_SHININESS(����ָ��),
	//GL_EMISSION(�Է���)
	//����ֻ��GL_SPECULAR��GL_SHININESS�������ã�������Ĭ��ֵ
	GLfloat mat_ambient[]={1,1,1,1};
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);//��������ɫ
	GLfloat mat_diffuse[]={1,1,1,1};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);//ɢ����ɫ
	glColor4f(1,1,1,1);//��ɫ����diffuse��color�󶨣������൱������diffuse��
	GLfloat mat_specular[]={0,0,0,1};
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);//������ɫ
	GLfloat mat_shininess[]={0};
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);//����ָ��
	GLfloat mat_emission[]={0.15,0.15,0.15,1};
	glMaterialfv(GL_FRONT,GL_EMISSION,mat_emission);//�Է���
	//----��ֹ������һ�����Ŷ���ɵĳ��ȸı䣬��glEnable(GL_NORMALIZE)Ч�ʸ�
	glDisable(GL_NORMALIZE);
	glEnable(GL_RESCALE_NORMAL);//ֻ�е���������������������Ϊһ�����ţ����������������������ͬ��
	                         //���ҷ������Ϊ��λ����ʱ���˷������ܵõ���ȷ���
	
	//----���ù�ԴLIGHT0
	//����8����ԴLIGHT0~LIGHT7
	//��������GL_AMBIENT,GL_DIFFUSE,GL_SPECULAR,GL_POSITION,GL_SPOT_DIRECTION,GL_SPOT_EXPONENT,GL_SPOT_CUTOFF,
	//GL_CONSTANT_ATTENUATION,GL_LINEAR_ATTENUATION,GL_QUADRATIC_ATTENUATION
	//����ֻ��GL_DIFFUSE��GL_SPECULAR�������ã�������Ĭ��ֵ
	GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);//������
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);//�����
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);//������

	//----���ù���ģ��
	//����GL_LIGHT_MODEL_AMBIENT,GL_LIGHT_MODEL_LOCAL_VIEWR,GL_LIGHT_MODEL_TWO_SIDE,GL_LIGHT_MODEL_COLOR_CONTROL
	//����ֻ��GL_LIGHT_MODEL_AMBIENT����һ�����ã�������Ĭ��ֵ
	GLfloat lmodel_ambient[]={0.5,0.5,0.5,1.0};
   	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
	glColor4f(1,1,1,1);
	//----������
	glFogi(GL_FOG_MODE,  GL_EXP2);   
    glFogfv(GL_FOG_COLOR, normalFogColor);                
    glFogf(GL_FOG_DENSITY,normalFogDesity);//���Ũ�ȣ�����ָ��ģʽ��    
//	//ע�⣬GL_FOG_START��GL_FOG_ENDֻ����������Ч����GL_EXP��GL_EXP2ģʽ��û�����õ�
//	glFogf(GL_FOG_START, 150);// �����Ŀ�ʼλ��
//  glFogf(GL_FOG_END, (rx+rz)/2*2);//��������λ��
	//----���ñ����޳�
	//ע�⣬cullFace�ǰ����㷽����˳ʱ�뻹����ʱ�����޳��ģ������ύ�ķ����������޹�
	glCullFace(GL_BACK);//���ر��棬����Ѳ�����ΪGL_FRONT���������档
	glDisable(GL_CULL_FACE);
	//------------------------����-----------------------------------
	glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);//glDepthFunc��Ĭ��ֵ����GL_LESS
	//---------------------------����Ƿ�֧��vbo��չ-----------------------------------------------------
	cout<<endl;
	cout<<"���ϵͳ�Ƿ�֧��vbo..."<<endl;
	supportVBO = IsExtensionSupported( "GL_ARB_vertex_buffer_object" ); 
	cout<<"������!"<<endl;
//	supportVBO=false;
	if(supportVBO){
		cout<<"ϵͳ֧��VBO!"<<endl;
	}else{
		cout<<"ϵͳ��֧��VBO!"<<endl;
		MessageBox(NULL,_T("ϵͳ��֧��VBO!"),
			_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		exit(0);//û��Ҫ�����������
	}
    //---------------------------����Ƿ�֧��fbo��չ-----------------------------------------------------
	cout<<endl;
	cout<<"���ϵͳ�Ƿ�֧��fbo..."<<endl;
	supportFBO = IsExtensionSupported( "GL_EXT_framebuffer_object" ); 
	cout<<"������!"<<endl;
//	supportFBO=false;
	if(supportFBO){
		cout<<"ϵͳ֧��FBO!"<<endl;
	}else{
		cout<<"ϵͳ��֧��FBO!"<<endl;
		MessageBox(NULL,_T("ϵͳ��֧��FBO!"),
			_T("ERROR"),MB_OK|MB_ICONEXCLAMATION);
		exit(0);//û��Ҫ�����������
	}

	//---------------------------Get Pointers To The GL Functions (һ��Ҫ���ڳ�ʼ��opengl֮�󣬲��ܱ�֤��õĺ���ָ�벻Ϊ��)
  
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
		//-------------������Ļfbo
	if(supportFBO){
        screenFBO.createFBO(512*2,512,2,1,
			GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR,
			GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR,
			//GL_LINEAR,GL_LINEAR
			GL_NEAREST,GL_NEAREST
			);
    }
/*	//------------------------------------------�ڵ����
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
	//--------------------------�����ڵ���ѯ
	glGenQueriesARB( 1, &occQuery );
	*/
	//---------------------------------shader-------------------------------------
	//����Ƿ�֧��opengl 2.0
	cout<<endl;
	cout<<"���ϵͳ�Ƿ�֧��openGL 2.0..."<<endl;
	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		cout<<"ϵͳ֧��openGL 2.0!"<<endl;
	else {
		cout<<"ϵͳ��֧��openGL 2.0!"<<endl;
		system("pause");
		exit(0);
	}
	//----------���أ�����shader����װshader����-------------------------------------------
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
	//------------------------����ȫ������---------------------------------------
	//��ÿ�����������ǰ�ȿ����Ƿ����ϵĲ��õ���������滻����
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
	//-----------------------����fbxģ��---------------------------------------
	//����fbxģ��
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
	//����ģ������
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
	//���ö���
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
	//---------------------------------�����ַ�����ʾ�б�------------------------------------------------------------------------
	//��NproteinMaxת�����ַ���
	{
		NproteinMax_str;
		char numbuff[20];//��ֵ�ִ�������
		_itoa_s(NproteinMax,numbuff,10);
		NproteinMax_str=numbuff;
	}
	//�����Ϊ��ʹ�ã���Ϊ��ռλ����ɾ
	CreateCNTextDispList("����:");//0
	CreateCNTextDispList("����:");//1
	CreateCNTextDispList("����δ���...");//2
	CreateCNTextDispList("���������,yeah!");//3
	CreateCNTextDispList("");//4
	CreateCNTextDispList("����:");//5
	CreateCNTextDispList("��Ծ");//6
	CreateCNTextDispList("�޵�");//7
	CreateCNTextDispList("����");//8
	CreateCNTextDispList("��·:");//9
	CreateCNTextDispList("");//10
	CreateCNTextDispList("��ͣ");//11
	CreateCNTextDispList("��Ϸ����!");//12
	CreateCNTextDispList("��C������,��������ʧ");//13
	CreateCNTextDispList("����'�˵�->����'�ر�����");//14
	CreateCNTextDispList("����'�˵�->����'�����м���");//15
	CreateCNTextDispList("���ף��ѿ���");//16
	CreateCNTextDispList("���ף��ѹر�");//17
	string tstr="�����ռ�";tstr+=NproteinMax_str;tstr+="����������";
	CreateCNTextDispList(tstr.c_str());//18
	//---------------------------------��ȡ��������----------------------------------------------------------------------------
	cout<<endl;
	cout<<"��ȡ��������..."<<endl;
	readLandMat();
	cout<<"��ȡ���!"<<endl;
	
	//---------------------------------����ӿڴ�С--------------------------------------------------------------------------
	//����ӿڴ�С
	GLint viewport[4];//4��Ԫ������ΪviewX,viewY,width,height
	glGetIntegerv(GL_VIEWPORT,viewport);
	widthEx=viewport[2];
	height=viewport[3];

	//---------------------------------���ӷ�Χ�������ο鳤��--------------------------------------------------------------
	areaHeight_god=331.371;//god���ӷ�Χ����(����ߴ�)
	areaWidth_god=331.371;
	blockWidth=areaWidth_god*8.67;//4;//ֻҪblockWidth>=areaWidth_god����
	blockHeight=areaHeight_god*8.67;//4;
	//���Χ����(���Χ��pcellΪ����)
	width_mov=areaHeight_god*3;
	height_mov=areaWidth_god*3;
	//����LOD�ο�����
	fakeDisMax=(width_mov+height_mov)/2;
	fakeDisMaxDiv6=fakeDisMax/6;
	fakeDisMaxDiv3=fakeDisMax/3;

	
	//---------------------------------�������ӿ�ground������------------------------------------------------------------
	//----����ground
	ground.init(blockWidth,blockHeight,-blockWidth/2,-blockHeight/2);
	ground.texture=tex_ground;
//	ground.visible=false;
	ground.setKuKv(0.005,0.005);
	ground.linkFrustum(&frustum);
	ground.fillNormalMat(landMat,normalMat);//���ɵ��η�������(��������趨�õ��ο��С֮��)
	//----����*pcell
	pcell=new CmyCell();
	cellRoot.addchild(pcell);
	pcell->setGround(&ground);
	pcell->setWater(&water);
	pcell->setV(pcell->vlen_slow,0,0);
	pcell->setRc(Rc_cell);
	init4(pcell->flglrcolor,1,70.0/255,70.0/255,1);
	pcell->bshell=true;
	pcell->bflglr=true;
	pcell->movloc((31366-32847)/10,2712.58/10+500,(13638.1-13080)/10);//��λpcell
	pcell->rotloc010(60+90+90);//����pcell����
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
	//���pcellλ��
	float cellpos[4];
	pcell->getPosInFather(cellpos);
	//�趨���λ��
	{
		float campos_incell[4]={0,320,600,1};//ϸ������ϵ�������λ��
		pcell->toFather_wOne(campos_incell,frustum.campos);//�õ�frustum.campos
	}
	//----����house
/*	//��house_temp.modelfbx�ж�ȡΨһ��mesh
	{
		Cmesh*pmesh=NULL;
		house_temp.modelfbx.show_ToMesh(pmesh);
		pmesh->calTextureInfo();
		pmesh->indexVBO.makeVBO(pmesh->vlist,pmesh->IDtriList,pmesh->textureList,pmesh->texIDtriCountList,GL_TRIANGLES);
		house.pmesh=pmesh;
	}*/
	//��house_temp.modelfbx�ж�ȡ����mesh�����������Ժ��ٺϲ�
	{
		vector<Cmesh*> pmeshList;//mesh�б�
		house_temp.modelfbx.show_ToMeshList(pmeshList);//�õ�pmeshList
		//��pmeshList�и�mesh�ϲ���һ��mesh�����ϲ���pmeshList[0]��
		{
			int nMesh=(int)pmeshList.size();
			for(int i=1;i<nMesh;i++){//ע���Ǵ�1��ʼ��
				Cmesh*pmesh=pmeshList[i];
				pmeshList[0]->merge(*pmesh);
			}
		}
		//ɾ�������mesh��pmeshList[1~nMesh-1]ɾ����
		{
			int nMesh=(int)pmeshList.size();
			for(int i=1;i<nMesh;i++){//ע���Ǵ�1��ʼ��
				Cmesh*pmesh=pmeshList[i];
				delete pmesh;
			}
		}
		//��Ψһ��mesh����house.pmesh
		house.pmesh=pmeshList[0];
		//����house.pmesh
		house.pmesh->calTextureInfo();
		house.pmesh->indexVBO.makeVBO(house.pmesh->vlist,house.pmesh->IDtriList,house.pmesh->textureList,house.pmesh->texIDtriCountList,GL_TRIANGLES);
	}
	house.makeOctree();
	house.setPcell(pcell);
	house.linkFrustum(&frustum);
	//----����metalBall
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
	{//Ϊ������������ʾ�б�
		GLuint displist=displistManager.genDisplist();
		glNewList(displist,GL_COMPILE);
		ball_useNormalAsUV(13,20.0);
		glEndList();
		//����metalBall
		metalBall.displist=displist;
	}
	//----������պ�
	float rx,rz,ry;
	rx=areaWidth_god*6*2.3;
	rz=areaHeight_god*6*2.3;
	ry=(rx+rz)/2;
    skyball.zoomloc(rx,ry,rz);
	skyball_temp.modelfbx.show_ToMesh(skyball.pmesh);
	skyball.pmesh->calTextureInfo();
	skyball.pmesh->directVBO_singleTex.makeVBO(skyball.pmesh->vlist,skyball.pmesh->IDtriList,GL_TRIANGLES);
	//----����water
	water.init(Rc_cell*6,Rc_cell*6);
	water.texture=tex_water;
	water.setHorizon(waterHorizon);
	//----����axis
	ground.addchild(&axis);
	axis.setLenXYZ(1000,1000,1000);
	axis.movloc(0,50,0);
	axis.visible=false;
	//----����pproteinList
	{
		//����������Ʒ�б�
		const int nSample=1;
		Cmesh*pmeshSampleList_protein[nSample];//������Ʒ�б�
		protein_temp.modelfbx.show_ToMesh(pmeshSampleList_protein[0]);
		pmeshSampleList_protein[0]->calTextureInfo();
		pmeshSampleList_protein[0]->directVBO_singleTex.makeVBO(pmeshSampleList_protein[0]->vlist,pmeshSampleList_protein[0]->IDtriList,GL_TRIANGLES);
		//����proteinList
		float proteinPosList[NproteinMax][4]={//proteinλ���б�
			{(34317.3-32847)/10+20  ,(4691.76+200)/10+50,(20961.5-13080)/10-185,1},//����
			{244,294+45,69,1},//���
			{(25957.9-32847)/10+20+90  ,(5336.5)/10+20,(20823.2-13080)/10-180,1},//����̨��
			{(31204.5-32847)/10+20  ,(2981.88+350+200)/10,(5790.72-13080)/10+10,1},//���
			{255,1662+40,-1967,1},//����С��
			{-296,2275+45,-1642,1}//�����
		};
		pproteinList.reserve(NproteinMax);//Ԥ��
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
	
	//----����modelx
	modelx.setGround(&ground);
	modelx.modelfbx.rotFbxScene_general(1,0,0,-90);
	modelx.modelfbx.rotFbxScene_general(0,0,1,180);
	modelx.modelfbx.movFbxScene(0,0,-110);//�����ƶ�һЩ������ǰ����ת������+ZΪ���ϣ�������ƶ����Ƿ���ǰ�ģ�����Ӧ�ϴ�
	modelx.movloc((22915-32847)/10+20  ,306/10,(12383-13080)/10-135);
	modelx.rotloc010(-90-60+45);
	modelx.HaboveGround=11.5;
	modelx.setRc(13.0);
//	modelx.bshowBB=true;
	modelx.zoomloc(0.1,0.1,0.1);//�ӵ�
	fbxModelRoot.addchild(&modelx);

	//----����modelx2
	modelx2.setGround(&ground);
	modelx2.modelfbx.movFbxScene(0,0,-4.0);//�����ƶ�һЩ������ǰ����ת������+ZΪ���ϣ�
	modelx2.movloc((32591-20-32847)/10+50,(820+130-160)/10,(10357.1-30-13080)/10-52.5);
	modelx2.setRc(18.5);
//	modelx2.bshowBB=true;
	modelx2.zoomloc(0.43,0.43,0.43);
	fbxModelRoot.addchild(&modelx2);

	//----����pdiedsuperburstList
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
	//----����pdiedautoCellList�б�
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
			//ֻ���뵽�����о��б�
			pdiedautoCellList.push_back(pautoCell);
		}
	}
	
	//----����tree
	{
		//����������Ʒ�б�
		const int nSample=3;
		Cmesh*pmeshSampleList_tree[nSample];//������Ʒ�б�
		tree0_temp.modelfbx.show_ToMesh(pmeshSampleList_tree[0]);
		tree1_temp.modelfbx.show_ToMesh(pmeshSampleList_tree[1]);
		tree2_temp.modelfbx.show_ToMesh(pmeshSampleList_tree[2]);
		pmeshSampleList_tree[0]->calTextureInfo();
		pmeshSampleList_tree[1]->calTextureInfo();
		pmeshSampleList_tree[2]->calTextureInfo();
		pmeshSampleList_tree[0]->directVBO_singleTex.makeVBO(pmeshSampleList_tree[0]->vlist,pmeshSampleList_tree[0]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_tree[1]->directVBO_singleTex.makeVBO(pmeshSampleList_tree[1]->vlist,pmeshSampleList_tree[1]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_tree[2]->directVBO_singleTex.makeVBO(pmeshSampleList_tree[2]->vlist,pmeshSampleList_tree[2]->IDtriList,GL_TRIANGLES);
		//����ptreeList
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
			//��½
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
	//----����pdiedamacList
	{
		//����������Ʒ�б�
		const int nSample=3;
		Cmesh*pmeshSampleList_amac[nSample];//������Ʒ�б�
		amac_r_temp.modelfbx.show_ToMesh(pmeshSampleList_amac[0]);
		amac_g_temp.modelfbx.show_ToMesh(pmeshSampleList_amac[1]);
		amac_b_temp.modelfbx.show_ToMesh(pmeshSampleList_amac[2]);
		pmeshSampleList_amac[0]->calTextureInfo();
		pmeshSampleList_amac[1]->calTextureInfo();
		pmeshSampleList_amac[2]->calTextureInfo();
		pmeshSampleList_amac[0]->directVBO_singleTex.makeVBO(pmeshSampleList_amac[0]->vlist,pmeshSampleList_amac[0]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_amac[1]->directVBO_singleTex.makeVBO(pmeshSampleList_amac[1]->vlist,pmeshSampleList_amac[1]->IDtriList,GL_TRIANGLES);
		pmeshSampleList_amac[2]->directVBO_singleTex.makeVBO(pmeshSampleList_amac[2]->vlist,pmeshSampleList_amac[2]->IDtriList,GL_TRIANGLES);
		//����pdiedamacList
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
	//----����pdiedeneList
	{
		//����������Ʒ�б�
		const int nSample=1;
		Cmesh*pmeshSampleList_ene[nSample];//������Ʒ�б�
		ene_r_temp.modelfbx.show_ToMesh(pmeshSampleList_ene[0]);
		pmeshSampleList_ene[0]->calTextureInfo();
		pmeshSampleList_ene[0]->directVBO_singleTex.makeVBO(pmeshSampleList_ene[0]->vlist,pmeshSampleList_ene[0]->IDtriList,GL_TRIANGLES);
		//����peneList
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
	//----����pdiedgeneList
	{
		//����������Ʒ�б�
		const int nSample=4;
		Cmesh*pmeshSampleList_gene[nSample];//������Ʒ�б�
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
		//����pdiedgeneList
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
	//----����dropColorList
	{
		Crgba rgba;
		rgba.init(1,0,0);
		dropColorList.push_back(rgba);
		rgba.init(0,1,0);
		dropColorList.push_back(rgba);
		rgba.init(0,0,1);
		dropColorList.push_back(rgba);

	}
	//----����pdieddropList
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
	//----����pdiedbubbleList
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
	//----����pdiedburstList
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
			pbigcannon->sourceMake(2);//������Դ
			pbigcannon->setRc(Rc_bigcannon);
			//	pbigcannon->bshowBB=true;
			pbigcannon->pmesh=pmesh;
		}
	}
	//--------------------------------------��ʼ����ײ������--------------------------------------
	{
		//��ʼ��autoCell����ײ������
		autoCellCgrid.setFrame(width_mov,height_mov,max(Rc_autoCell,Rc_cell)*2,max(Rc_autoCell,Rc_cell)*2);
		autoCellCgrid.reserve_clist(NautoCell);
		//��ʼ��amac����ײ������
		amacCgrid.setFrame(width_mov,height_mov,max(Rc_cell,Rc_amac)*2,max(Rc_cell,Rc_amac)*2);
		amacCgrid.reserve_clist(Namac);
		//��ʼ��ene����ײ������
		eneCgrid.setFrame(width_mov,height_mov,max(Rc_cell,Rc_ene)*2,max(Rc_cell,Rc_ene)*2);
		eneCgrid.reserve_clist(Nene);
		//��ʼ��gene����ײ������
		geneCgrid.setFrame(width_mov,height_mov,max(Rc_cell,Rc_gene)*2,max(Rc_cell,Rc_gene)*2);
		geneCgrid.reserve_clist(Ngene);
		//��ʼ��tree����ײ������
		treeCgrid.setFrame(width_mov,height_mov,max(Rc_cell,Rc_tree)*2,max(Rc_cell,Rc_tree)*2);
		treeCgrid.reserve_clist(Ntree);
	}
	//--------------------�����ڵ㺢������reserve----------------------------------
	//fbxModelRoot.p.reserve();
	//proteinRoot.p.reserve();
	treeRoot.p.reserve(Ntree);
	superburstRoot.p.reserve(Nsuperburst);
	bigcannonRoot.p.reserve(Nbigcannon);
	burstRoot.p.reserve(Nburst);
	dropRoot.p.reserve(Ndrop);
	bubbleRoot.p.reserve(Nbubble);
	molecuRoot.p.reserve(Namac+Nene+Ngene);
	cellRoot.p.reserve(NautoCell+1);//1����pcell
	//--------------------ɾ������_tempģ��
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
	//--------------��߽������ȼ�-----------------------------------------------------------------
	
	SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);
	//-------------------------------------------------------------------------------------------
	return true;

}