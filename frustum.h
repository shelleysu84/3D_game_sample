#ifndef CFRUSTUM_H_H
#define CFRUSTUM_H_H
class Cfrustum{
public:
	float campos[4];
	GLint viewport[4];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	Cplane planeList[6];//��׶��ʾΪ����ƽ�棨��һ��Ϊ��ƽ�棬���һ��ΪԶƽ�棬�м��ĸ�Ϊ���棩
	void update(){
		glGetDoublev(GL_MODELVIEW_MATRIX  ,modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
		glGetIntegerv(GL_VIEWPORT,viewport);
		//////////////////////////////////////////////
		float winwidth=viewport[2];
		float winheight=viewport[3];
		//��ƽ��
		//   (0,winheight,0)---(winwidth,winheight,0)
		//      |              |
		//      |              |
		//    (0,0,0)--------(winwidth,0,0)
		//Զƽ��
		//   (0,winheight,1)---(winwidth,winheight,1)
		//      |              |
		//      |              |
		//    (0,0,1)--------(winwidth,0,1)
		float p[4][4];//��ƽ���ĸ�������ӿڿռ�����
		init4(p[0],0,0,0,1);
		init4(p[1],winwidth,0,0,1);
		init4(p[2],winwidth,winheight,0,1);
		init4(p[3],0,winheight,0,1);
		//��pת��������ռ�
		for(int i=0;i<=3;i++){
			//��p[i]ת��������ռ�
			GLdouble tp[3];
			gluUnProject(p[i][X],p[i][Y],p[i][Z],modelMatrix,projMatrix,viewport,&tp[X],&tp[Y],&tp[Z]);
			init4(p[i],tp[X],tp[Y],tp[Z],1);
		}//p��ת��Ϊ��������
		float q[4]={0,0,1,1};//Զƽ���ϵ�һ����(�ӿڿռ�)
		//��qת��������ռ�
		GLdouble tp[3];
		gluUnProject(q[X],q[Y],q[Z],modelMatrix,projMatrix,viewport,&tp[X],&tp[Y],&tp[Z]);
		init4(q,tp[X],tp[Y],tp[Z],1);//q��ת��������ռ�
		//�������ƽ�棨����ָ����׶�ڲ���
		//������ķ�����
		float norm[6][4];//��ǰ�汾�д˴���дΪ��norm[5][4]
		getNormalOfTri(p[2],p[1],p[0],norm[0]);//��ƽ�淨��
		getNormalOfTri(campos,p[1],p[0],norm[1]);
		getNormalOfTri(campos,p[2],p[1],norm[2]);
		getNormalOfTri(campos,p[3],p[2],norm[3]);
		getNormalOfTri(campos,p[0],p[3],norm[4]);
		neg(norm[0],norm[5]);//Զƽ�淨�����ƽ���෴��
		//��������
		planeList[0].makePlane(p[0],norm[0]);//��ƽ��
		planeList[1].makePlane(campos,norm[1]);
		planeList[2].makePlane(campos,norm[2]);
		planeList[3].makePlane(campos,norm[3]);
		planeList[4].makePlane(campos,norm[4]);
		planeList[5].makePlane(q,norm[5]);//Զƽ��
	}
};
#endif
