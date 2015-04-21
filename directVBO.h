#ifndef CDIRECTVBO_SINGLETEX_H_H
#define CDIRECTVBO_SINGLETEX_H_H
class CdirectVBO_singleTex{//������ֱ��vbo
public:
	GLuint VertexGBuffer; 
	GLuint NormGBuffer; 
	GLuint TexCoordGBuffer;
	int vertexCount;//������
	GLenum mode;//��״
	CdirectVBO_singleTex(){
		mode=GL_TRIANGLES;
		vertexCount=0;
		VertexGBuffer = 0; 
		NormGBuffer = 0; 
		TexCoordGBuffer =0;
	}
	~CdirectVBO_singleTex(){
		//����vbo
		if(VertexGBuffer!=0)glDeleteBuffersARB(1,&VertexGBuffer);
		if(NormGBuffer!=0)glDeleteBuffersARB(1,&NormGBuffer);
		if(TexCoordGBuffer!=0)glDeleteBuffersARB(1,&TexCoordGBuffer);
		
	}
	void showGBuffer(GLuint texture){
		glBindTexture(GL_TEXTURE_2D,texture);
		//����
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, VertexGBuffer ); 
		glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );//ָ������󶨵��Ƕ��㣬��ָ�������ʽ
		//����
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NormGBuffer );//ָ������󶨵��Ƿ��ߣ���ָ�����߸�ʽ
		glNormalPointer( GL_FLOAT, 0, (char *) NULL ); 
		//uv
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, TexCoordGBuffer );//ָ������󶨵����������꣬��ָ����������ĸ�ʽ
		glTexCoordPointer(2,GL_FLOAT, 0, (char *) NULL ); 
		
		glDrawArrays(mode,0,vertexCount);

		glBindBufferARB(GL_ARRAY_BUFFER, 0); //���
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
	void submitVertexs(vector<Cvertex>&vlist,vector<CIDTriangle>&IDtriList){//��mesh�ж��������ύ���Կ�,������mesh������ִ��
		vertexCount=(int)vlist.size();
		//-----------------------------------
		vector<float> vertexArray;//�Ƶ㻺������
		vertexArray.reserve(vertexCount*3);
		vector<float> uvArray;//uv��������
		uvArray.reserve(vertexCount*2);
		vector<float> normArray;//���򻺳�����
		normArray.reserve(vertexCount*3);
		//------------------------������������
		//�������㻺�����飬���򻺳������uv��������
		
		//��������������
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
		//-------------------����vbo
		//��������vbo
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, VertexGBuffer);
		if((int)vertexArray.size()>0){
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(float)*(int)vertexArray.size(), &vertexArray[0], GL_STATIC_DRAW_ARB ); 
		}
		glBindBufferARB(GL_ARRAY_BUFFER, 0);//��󶨣�ʹ����������ʽ�Ļ滭����Ӱ�죩
		//��ʱ����vbo�ѽ������������������ύ�Կ�������ʹvertexArray�����޸�Ҳ��Ӱ�졣
		//��������vbo
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, NormGBuffer);
		if((int)normArray.size()>0){
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)*(int)normArray.size(), &normArray[0], GL_STATIC_DRAW_ARB ); 
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		//����uv��vbo
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, TexCoordGBuffer);
		if((int)uvArray.size()>0){
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)*(int)uvArray.size(), &uvArray[0], GL_STATIC_DRAW_ARB ); 
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
	
	

};
#endif