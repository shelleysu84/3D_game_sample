
inline void ball_useNormalAsUV(int m,float r)//�����壨���Ż���
{//rΪ��뾶,nΪ�������,mΪ����
	int n=m/2;
	int _2n=2*n;//2*n
	//�����
	float A,B;
	//A,B����
	float dA=2*pi/m;
	float dB=pi/_2n;
	//A,B��ֵ
	float A0=-dA;
	float B0=-halfpi-dB;
	glBegin(GL_QUADS);
	A=A0;
	float cosA_store,sinA_store;
	for(int i=0;i<m;i++){
		A+=dA;
		B=B0;
		float cosA,sinA,cosAplus,sinAplus;
		if(i==0){
			cosA=fastcos(A);
			sinA=fastsin(A);
		}else{
			cosA=cosA_store;
			sinA=sinA_store;
		}
		cosAplus=fastcos(A+dA);
		sinAplus=fastsin(A+dA);
		//�洢
		cosA_store=cosAplus;
		sinA_store=sinAplus;
		float cosB_store,sinB_store;
		for(int j=0;j<_2n;j++){	
			B+=dB;
			//���Ǻ���ֵ
			float cosB,sinB,cosBplus,sinBplus;
			if(j==0){
				cosB=fastcos(B);
				sinB=fastsin(B);
			}else{
				cosB=cosB_store;
				sinB=sinB_store;
			}
			
			cosBplus=fastcos(B+dB);
			sinBplus=fastsin(B+dB);
			
			//�洢
			cosB_store=cosBplus;
			sinB_store=sinBplus;
			//���ı���(A,B)->(A,B+dB)->(A+dA,B+dB)->(A+dA,B)->(A,B)
			float x,y,z;//��������
			float nx,ny,nz;//������
			//(A,B)
			nx=cosB*cosA;ny=sinB;nz=cosB*sinA;
			x=r*nx;y=r*ny;z=r*nz;
			glTexCoord3f(nx,ny,nz);
			glNormal3f(nx,ny,nz);glVertex3f(x,y,z);
			//(A,B+dB)
			nx=cosBplus*cosA;ny=sinBplus;nz=cosBplus*sinA;
			x=r*nx;y=r*ny;z=r*nz;
			glTexCoord3f(nx,ny,nz);
			glNormal3f(nx,ny,nz);glVertex3f(x,y,z);
			//(A+dA,B+dB)
			nx=cosBplus*cosAplus;ny=sinBplus;nz=cosBplus*sinAplus;
			x=r*nx;y=r*ny;z=r*nz;
			glTexCoord3f(nx,ny,nz);
			glNormal3f(nx,ny,nz);glVertex3f(x,y,z);
			//(A+dA,B)
			nx=cosB*cosAplus;ny=sinB;nz=cosB*sinAplus;
			x=r*nx;y=r*ny;z=r*nz;
			glTexCoord3f(nx,ny,nz);
			glNormal3f(nx,ny,nz);glVertex3f(x,y,z); 
		}
	}
	glEnd();
}	


inline void ball_wire(float r)//�����壨δ�Ż���
{//rΪ��뾶,nΪ�������,mΪ����
	float n=4;
	float m=8;
	for(int i=0;i<=m-1;i++){
		for(int j=0;j<=2*n-1;j++){
			float A=i*2*pi/m;//��x��нǣ����ȣ�
			float B=-halfpi+j*halfpi/n;//��XZƽ��нǣ����ȣ�
			float dA=2*pi/m;
			float dB=pi/(2*n);
			//���ı���(A,B)->(A,B+dB)->(A+dA,B+dB)->(A+dA,B)->(A,B)
			float x1=r*fastcos(B)*fastcos(A);float y1=r*fastsin(B);float z1=r*fastcos(B)*fastsin(A);//(A,B)
			float x2=r*fastcos(B+dB)*fastcos(A);float y2=r*fastsin(B+dB);float z2=r*fastcos(B+dB)*fastsin(A);//(A,B+dB)
			float x3=r*fastcos(B+dB)*fastcos(A+dA);float y3=r*fastsin(B+dB);float z3=r*fastcos(B+dB)*fastsin(A+dA);//(A+dA,B+dB)
			float x4=r*fastcos(B)*fastcos(A+dA);float y4=r*fastsin(B);float z4=r*fastcos(B)*fastsin(A+dA);//(A+dA,B)
			glBegin(GL_LINES);

			glVertex3f(x1,y1,z1);
			glVertex3f(x2,y2,z2);

			glVertex3f(x2,y2,z2);
			glVertex3f(x3,y3,z3);

			glVertex3f(x3,y3,z3);
			glVertex3f(x4,y4,z4);

			glVertex3f(x4,y4,z4);
			glVertex3f(x1,y1,z1);
 
			glEnd();
			
		}
	}
}	

inline void drawRectWire(const float p[4][3],const float color[3],const float width){//������Ϊp[0]~p[3]���ı��α߿�,u[i]Ϊ��Ӧ��ͼ����
		//  p0-p3
		//  |   |
		//  p1-p2

		//���߿�
		glColor3fv(color);
		glLineWidth(width);
	    glBegin(GL_LINES);

		glVertex3f(p[0][X],p[0][Y]+0.6,p[0][Z]);
		glVertex3f(p[1][X],p[1][Y]+0.6,p[1][Z]);

		glVertex3f(p[1][X],p[1][Y]+0.6,p[1][Z]);
		glVertex3f(p[2][X],p[2][Y]+0.6,p[2][Z]);
	    
		glVertex3f(p[2][X],p[2][Y]+0.6,p[2][Z]);
		glVertex3f(p[3][X],p[3][Y]+0.6,p[3][Z]);

		glVertex3f(p[3][X],p[3][Y]+0.6,p[3][Z]);
		glVertex3f(p[0][X],p[0][Y]+0.6,p[0][Z]);
	
		glEnd();
		glColor3f(1,1,1);


	}

