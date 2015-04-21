#ifndef CFRUSTUM_H_H
#define CFRUSTUM_H_H
class Cfrustum{
public:
	float campos[4];
	GLint viewport[4];
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	Cplane planeList[6];//视锥表示为六个平面（第一个为近平面，最后一个为远平面，中间四个为侧面）
	void update(){
		glGetDoublev(GL_MODELVIEW_MATRIX  ,modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
		glGetIntegerv(GL_VIEWPORT,viewport);
		//////////////////////////////////////////////
		float winwidth=viewport[2];
		float winheight=viewport[3];
		//近平面
		//   (0,winheight,0)---(winwidth,winheight,0)
		//      |              |
		//      |              |
		//    (0,0,0)--------(winwidth,0,0)
		//远平面
		//   (0,winheight,1)---(winwidth,winheight,1)
		//      |              |
		//      |              |
		//    (0,0,1)--------(winwidth,0,1)
		float p[4][4];//近平面四个顶点的视口空间坐标
		init4(p[0],0,0,0,1);
		init4(p[1],winwidth,0,0,1);
		init4(p[2],winwidth,winheight,0,1);
		init4(p[3],0,winheight,0,1);
		//将p转化到世界空间
		for(int i=0;i<=3;i++){
			//将p[i]转化到世界空间
			GLdouble tp[3];
			gluUnProject(p[i][X],p[i][Y],p[i][Z],modelMatrix,projMatrix,viewport,&tp[X],&tp[Y],&tp[Z]);
			init4(p[i],tp[X],tp[Y],tp[Z],1);
		}//p已转化为世界坐标
		float q[4]={0,0,1,1};//远平面上的一个点(视口空间)
		//将q转化到世界空间
		GLdouble tp[3];
		gluUnProject(q[X],q[Y],q[Z],modelMatrix,projMatrix,viewport,&tp[X],&tp[Y],&tp[Z]);
		init4(q,tp[X],tp[Y],tp[Z],1);//q已转化到世界空间
		//求这五个平面（法向指向视锥内部）
		//六个面的法向量
		float norm[6][4];//以前版本中此处误写为了norm[5][4]
		getNormalOfTri(p[2],p[1],p[0],norm[0]);//近平面法向
		getNormalOfTri(campos,p[1],p[0],norm[1]);
		getNormalOfTri(campos,p[2],p[1],norm[2]);
		getNormalOfTri(campos,p[3],p[2],norm[3]);
		getNormalOfTri(campos,p[0],p[3],norm[4]);
		neg(norm[0],norm[5]);//远平面法向（与近平面相反）
		//求六个面
		planeList[0].makePlane(p[0],norm[0]);//近平面
		planeList[1].makePlane(campos,norm[1]);
		planeList[2].makePlane(campos,norm[2]);
		planeList[3].makePlane(campos,norm[3]);
		planeList[4].makePlane(campos,norm[4]);
		planeList[5].makePlane(q,norm[5]);//远平面
	}
};
#endif
