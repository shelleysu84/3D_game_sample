#ifndef CDIRECTVBO_SINGLETEX_H_H
#define CDIRECTVBO_SINGLETEX_H_H
class CdirectVBO_singleTex{//单纹理直接vbo
public:
	GLuint VertexGBuffer; 
	GLuint NormGBuffer; 
	GLuint TexCoordGBuffer;
	int vertexCount;//顶点数
	GLenum mode;//形状
	CdirectVBO_singleTex(){
		mode=GL_TRIANGLES;
		vertexCount=0;
		VertexGBuffer = 0; 
		NormGBuffer = 0; 
		TexCoordGBuffer =0;
	}
	~CdirectVBO_singleTex(){
		//销毁vbo
		if(VertexGBuffer!=0)glDeleteBuffersARB(1,&VertexGBuffer);
		if(NormGBuffer!=0)glDeleteBuffersARB(1,&NormGBuffer);
		if(TexCoordGBuffer!=0)glDeleteBuffersARB(1,&TexCoordGBuffer);
		
	}
	void showGBuffer(GLuint texture){
		glBindTexture(GL_TEXTURE_2D,texture);
		//顶点
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, VertexGBuffer ); 
		glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );//指明上面绑定的是顶点，并指明顶点格式
		//法向
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NormGBuffer );//指明上面绑定的是法线，并指明法线格式
		glNormalPointer( GL_FLOAT, 0, (char *) NULL ); 
		//uv
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, TexCoordGBuffer );//指明上面绑定的是纹理坐标，并指明纹理坐标的格式
		glTexCoordPointer(2,GL_FLOAT, 0, (char *) NULL ); 
		
		glDrawArrays(mode,0,vertexCount);

		glBindBufferARB(GL_ARRAY_BUFFER, 0); //解绑定
	}
	
	void makeVBO(vector<Cvertex>&vlist,vector<CIDTriangle>&IDtriList,GLenum _mode){
		genGBuffers();
		submitVertexs(vlist,IDtriList);
		mode=_mode;
	}
	void genGBuffers(){
		glGenBuffersARB(1,&VertexGBuffer);
		glGenBuffersARB(1,&NormGBuffer);
		glGenBuffersARB(1,&TexCoordGBuffer);
		
	}
	void submitVertexs(vector<Cvertex>&vlist,vector<CIDTriangle>&IDtriList){//将mesh中顶点数据提交到显卡,必须在mesh建立后执行
		vertexCount=(int)vlist.size();
		//-----------------------------------
		vector<float> vertexArray;//制点缓冲数组
		vertexArray.reserve(vertexCount*3);
		vector<float> uvArray;//uv缓冲数组
		uvArray.reserve(vertexCount*2);
		vector<float> normArray;//法向缓冲数组
		normArray.reserve(vertexCount*3);
		//------------------------制作缓冲数组
		//制作顶点缓冲数组，法向缓冲数组和uv缓冲数组
		
		//制作各缓冲数组
		int IDtriCount=vertexCount/3;
		if(IDtriCount!=(int)IDtriList.size()){
			cout<<"IDtriCount!=IDtriList.size()!"<<endl;
			system("pause");
			exit(0);
		}
		for(int i=0;i<IDtriCount;i++){
			CIDTriangle&IDtri=IDtriList[i];
			for(int j=0;j<3;j++){
				float*v=vlist[IDtri.vID[j]].v;
				float*norm=IDtri.norm;
				float*u=vlist[IDtri.vID[j]].u;
				//vertexArray
				vertexArray.push_back(v[X]);
				vertexArray.push_back(v[Y]);
				vertexArray.push_back(v[Z]);
				//normArray
				normArray.push_back(norm[X]);
				normArray.push_back(norm[Y]);
				normArray.push_back(norm[Z]);
				//uvArray
				uvArray.push_back(u[0]);
				uvArray.push_back(u[1]);
			}
		
		}
		//-------------------制作vbo
		//制作顶点vbo
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, VertexGBuffer);
		if((int)vertexArray.size()>0){
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*(int)vertexArray.size(), &vertexArray[0], GL_STATIC_DRAW_ARB ); 
		}
		glBindBufferARB(GL_ARRAY_BUFFER, 0);//解绑定（使后面其它形式的绘画不受影响）
		//此时顶点vbo已建立（即顶点数据已提交显卡），即使vertexArray内容修改也不影响。
		//制作法向vbo
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, NormGBuffer);
		if((int)normArray.size()>0){
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)*(int)normArray.size(), &normArray[0], GL_STATIC_DRAW_ARB ); 
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		//制作uv的vbo
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, TexCoordGBuffer);
		if((int)uvArray.size()>0){
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)*(int)uvArray.size(), &uvArray[0], GL_STATIC_DRAW_ARB ); 
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
	
	

};
#endif