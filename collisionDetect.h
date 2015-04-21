
#ifndef CCOLLISIONPAIR_H_H
#define CCOLLISIONPAIR_H_H
class CcollisionPair{
public:
	Cmodel* pmodel1,*pmodel2;
	float pos1[4],pos2[4];
	float R1,R2;
	float n1[4],n2[4];
	float dir1[4],dir2[4];
	float d;
	CcollisionPair(){
		pmodel1=NULL;
		pmodel2=NULL;
	}
	void setPmodel(Cmodel*_pmodel1,Cmodel*_pmodel2){
		pmodel1=_pmodel1;
		pmodel2=_pmodel2;
	}
	void setDir(float *_dir1,float *_dir2){
		veccopy(_dir1,dir1);
		veccopy(_dir2,dir2);
	}
	void setNorm(float*_n1,float *_n2){
		veccopy(_n1,n1);
		veccopy(_n2,n2);
	}
	void setR(float _R1,float _R2){
		R1=_R1;
		R2=_R2;
	}
	void setPos(float*_pos1,float*_pos2){
		veccopy(_pos1,pos1);
		veccopy(_pos2,pos2);
	}
	void setD(float _d){
		d=_d;
	}

	void set(Cmodel*_pmodel1,Cmodel*_pmodel2,
	float*_pos1=NULL,float _R1=0,float*_pos2=NULL,float _R2=0,float*_n1=NULL,float*_n2=NULL,float _d=0,
	float*_dir1=NULL,float*_dir2=NULL){
		pmodel1=_pmodel1;
		pmodel2=_pmodel2;
		if(_pos1!=NULL)veccopy(_pos1,pos1);
		if(_pos2!=NULL)veccopy(_pos2,pos2);
		R1=_R1;
		R2=_R2;
		if(_n1!=NULL)veccopy(_n1,n1);
		if(_n2!=NULL)veccopy(_n2,n2);
		d=_d;
		if(_dir1!=NULL)veccopy(_dir1,dir1);
		if(_dir2!=NULL)veccopy(_dir2,dir2);
	}
	
};
#endif
#ifndef CCOLLISIONTESTPROPERTY_H_H
#define CCOLLISIONTESTPROPERTY_H_H
class CcollisionTestProperty{
public:
	bool getCollisionDeepth;//�Ƿ��ô������
	bool getCollisionNormals;//�Ƿ�����ײʱ���ߵķ���
	bool getCollisionDirs;//�Ƿ�����ײʱ���߳���
	bool getCollisionPoses;//�Ƿ�����ײʱ����λ��
	bool getCollisionRs;//�Ƿ�����ײʱ���ߵİ뾶
	CcollisionTestProperty(){
		getCollisionDeepth=true;
		getCollisionNormals=true;
		getCollisionDirs=true;
		getCollisionPoses=true;
		getCollisionRs=true;
	}
	void set(bool _getCollisionDeepth=true,bool _getCollisionNormals=true,
		bool _getCollisionDirs=true,bool _getCollisionPoses=true,bool _getCollisionRs=true){
		getCollisionDeepth=_getCollisionDeepth;
		getCollisionNormals=_getCollisionNormals;
		getCollisionDirs=_getCollisionDirs;
		getCollisionPoses=_getCollisionPoses;
		getCollisionRs=_getCollisionRs;
	}
};
#endif
#ifndef CCOLLISIONREACTPROPERTY_H_H
#define CCOLLISIONREACTPROPERTY_H_H
class CcollisionReactProperty{
public:
	bool ignorY;//�Ƿ����Y����
	bool softBack;//�Ƿ������
	float k_softBack;//�����ϵ����ԽСԽ��Ϊ1���˻�ΪӲ���ˣ�ֻ�е�softBack==trueʱ��Ч
	CcollisionReactProperty(){
		ignorY=false;
		softBack=false;
		k_softBack=1;
	}
	void set(bool _ignorY=false,bool _softBack=false,float _k_softBack=1){
		ignorY=_ignorY;
		softBack=_softBack;
		k_softBack=_k_softBack;
	}
};
#endif


//--------------------------------------------------------------------------------------
//						����������ײ�����صĺ���
//--------------------------------------------------------------------------------------

bool collisionTest_BallBall(const float c1[4],const float R1,//ball1
	const float c2[4],const float R2,//ball2
	const CcollisionTestProperty&ctp,//��ײ����
	CcollisionPair&cp)//���ص���ײ��Ϣ
//������֮�����ײ���
//�������ײ����n1,n2,d����Ч
{
	float dis2=pow2(c1[X]-c2[X])+pow2(c1[Y]-c2[Y])+pow2(c1[Z]-c2[Z]);
	bool collision=(dis2<=pow2(R1+R2));
	//������ײ��Ϣ
	if(collision){//��ײ
		//����������
		if(ctp.getCollisionDeepth){
			float d=R1+R2-CarmSqrt(dis2);
			cp.setD(d);
		}
		//���㻥������
		if(ctp.getCollisionNormals){
			float n1[4],n2[4];
			sub((float(&)[4])(*c1),(float(&)[4])(*c2),(float(&)[4])(*n1));
			if(normalize(n1)==0){//����������غϣ���������������涨������涨Ϊ(1,0,0,0)
				init4(n1,1,0,0,0);
			}
			neg(n1,n2);	
			cp.setNorm(n1,n2);
		}
		//��¼λ��
		if(ctp.getCollisionPoses){
			cp.setPos((float*)c1,(float*)c2);
			
		}
		return true;
	}else{//����ײ
		return false;
	}			
}


inline void collision_model_modelList(Cmodel*pmodel,
	vector<Cmodel*>&pModelList,
	Ccgrid&grid,
	const CcollisionTestProperty&ctp,//��ײ����
	const CcollisionReactProperty&crp,//��ײ��Ӧ����
	void (&collisionReact)(const CcollisionPair&,const CcollisionReactProperty&)//��ײ����
	)
{
	//�����ײ�б�
	float R1=pmodel->Rc;
	float pos1[4];
	pmodel->getPosInFather(pos1);
	float dir1[4];
	if(ctp.getCollisionDirs)
	{
		pmodel->getDirInFather(dir1);
	}
	grid.getclist(pos1[X],pos1[Z]);
	const vector<int>&clist=grid.clist;
	int nc=(int)clist.size();
	for(int i=0;i<nc;i++){
		int I=clist[i];//�±��
		Cmodel&model2=*pModelList[I];	
		if(model2.visible==false)continue;
		//���autoCell��myCell����ײ
		float pos2[4];//autoCellλ��
		model2.getPosInFather(pos2);
		float R2=model2.Rc;
		CcollisionPair cp;
		bool collision=collisionTest_BallBall(pos1,R1,pos2,R2,ctp,cp);
		if(collision){
			float dir2[4];
			if(ctp.getCollisionDirs){
				model2.getDirInFather(dir2);
				cp.setDir(dir1,dir2);
			}
			cp.setR(R1,R2);
			cp.setPmodel(pmodel,&model2);
			//������ײ��Ӧ
			collisionReact(cp,crp);
			break;//����ĳ�����󣬼�⵽����һ����ײ�����ˣ����涼��������

		}
	}
}
inline void collision_modelList_modelList(vector<Cmodel*>&pModelList1,
	vector<Cmodel*>&pModelList2,
	Ccgrid&grid2,
	const CcollisionTestProperty&ctp,//��ײ����
	const CcollisionReactProperty&crp,//��ײ��Ӧ����
	void (&collisionReact)(const CcollisionPair&,const CcollisionReactProperty&))//��ײ����
{
	//�����ж�pModelList1��pModelList2�Ƿ�Ϊͬһ����
	bool List1EqList2=(&pModelList1==&pModelList2);//ע�⣬Ӧ����&pModelList1==&pModelList2...(1)
											  //����Ҫ��pModelList1==pModelList2...(2)
	                                          //��1����ʾ�ж�pModelList1��pModelList2�Ƿ�Ϊͬһ������
											  //��2����ʾ�ж�pModelList1��pModelList2�Ƿ��ӦԪ�������Ԫ������ͬ
	int nModel=(int)pModelList1.size();
	for(int i=0;i<nModel;i++){
		Cmodel&model1=*pModelList1[i];
		if(model1.visible==false)continue;
		float pos1[4];
		model1.getPosInFather(pos1);
		float R1=model1.Rc;
		float dir1[4];
		if(ctp.getCollisionDirs)
		{
			model1.getDirInFather(dir1);
		}
		grid2.getclist(pos1[X],pos1[Z]);
		vector<int>&clist=grid2.clist;//��ײ�б�
		
		//model1��clist�е�autoCell�����ײ
		int nc=(int)clist.size();
		for(int j=0;j<nc;j++){
			int J=clist[j];//�±��
			if(List1EqList2&&J<=i)continue;//���pModelList1��pModelList2��ͬһ������
									    //��Ϊ�˱����ظ���⣬������model1�±��iС����ȵ��±��
			                            //���pModelList1��pModelList2����ͬһ������
										//��������
			Cmodel&model2=*pModelList2[J];
			if(model2.visible==false)continue;
			//���model1��model2����ײ
			float pos2[4];
			model2.getPosInFather(pos2);
			float R2=model2.Rc;
			CcollisionPair cp;
			bool collision=collisionTest_BallBall(pos1,R1,pos2,R2,ctp,cp);
			if(collision){
				float dir2[4];
				if(ctp.getCollisionDirs){
					model2.getDirInFather(dir2);
					cp.setDir(dir1,dir2);
				}
				cp.setR(R1,R2);
				cp.setPmodel(&model1,&model2);
				//������ײ��Ӧ
				collisionReact(cp,crp);
				break;//����ĳ�����󣬼�⵽����һ����ײ�����ˣ����涼��������

			}	
		}
	}


}

inline Cburst* layBurst(float x,float y,float z){//��ָ��λ�÷���һ����ը
	//���ڵ���ʧ������һ����ը
	int ndiedburst=(int)pdiedburstList.size();
	if(ndiedburst!=0){//���������ը�б��л���Ԫ��
		//��pdiedburstList��ȡһ�����뵽pburstList��
		Cburst&burst=*pdiedburstList[ndiedburst-1];
		pdiedburstList.pop_back();
		burst.reSet();
		pburstList.push_back(&burst);
		burstRoot.addchild(&burst);
		burst.setPosInFather(x,y,z);
		return (&burst);
	}else{
		return NULL;
	}

}
inline void collisionReact_BothRollAndBack(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	float A1=getA((float*)cp.dir1,(float*)cp.n1);
	float A2=getA((float*)cp.dir2,(float*)cp.n2);
	cp.pmodel1->rotloc010(A1);
	cp.pmodel2->rotloc010(A2);
	cp.pmodel1->movloc_001(-cp.d/2);
	cp.pmodel2->movloc_001(-cp.d/2);
}
inline void collisionReact_Back_RollAndBack(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel1����һ��
	float back[4];//pmodel1�Ļ�������
	mul(cp.d/2,cp.n1,back);//�õ�back
	//pmodel1����
	cp.pmodel1->movloc_inFather(back[X],back[Y],back[Z]);
	//pmodel2ת��
	float A2=getA((float*)cp.dir2,(float*)cp.n2);
	cp.pmodel2->rotloc010(A2);
	//pmodel2ǰ��һ��
	cp.pmodel2->movloc_001(-cp.d/2);

}
inline void collisionReact_Back_Back(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	float back[4];
	mul(cp.d/2,cp.n1,back);//�õ�back
	if(crp.ignorY)back[Y]=0;
	cp.pmodel1->movloc_inFather(back[X],back[Y],back[Z]);
	cp.pmodel2->movloc_inFather(-back[X],-back[Y],-back[Z]);
}

inline void collisionReact_Still_RollAndBack(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel2ת��
	float A2=getA((float*)cp.dir2,(float*)cp.n2);
	cp.pmodel2->rotloc010(A2);
	//pmodel2ǰ��һ��
	cp.pmodel2->movloc_001(-cp.d/2);

}

inline void collisionReact_Back_Still(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel1����
	float back[4];//��������
	mul(cp.d,cp.n1,back);
	if(crp.ignorY)back[Y]=0;//����Y����
	//��back����
	cp.pmodel1->movloc_inFather(back[X],back[Y],back[Z]);
}
inline void collisionReact_BackSoft_Still(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel1����
	float back[4];//��������
	mul(cp.d*crp.k_softBack,cp.n1,back);
	back[Y]=0;//����Y����
	//��back����
	cp.pmodel1->movloc_inFather(back[X],back[Y],back[Z]);

}

inline void collisionReact_Still_BackSoft(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel2����
	float back[4];//��������
	mul(cp.d*crp.k_softBack,cp.n2,back);
	back[Y]=0;//����Y����
	//��back����
	cp.pmodel2->movloc_inFather(back[X],back[Y],back[Z]);

}




//-------------------------------------------------------------------------------------------------
//             ���������д�÷ǳ�����������û��ͨ���ԣ�����ʱ��ϸ������ô��
//-------------------------------------------------------------------------------------------------



inline void collisionReact_pcell_autoCell(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	CmyCell&myCell=*(CmyCell*)cp.pmodel1;
	CautoCell&autoCell=*(CautoCell*)cp.pmodel2;
	if(myCell.nomatch==false){//���myCell���Ǵ����޵�״̬
		//myCell��������
		myCell.lifeplus(-1);
		collisionReact_Back_RollAndBack(cp,crp);
	}else{//���myCell�����޵�״̬
		//autoCell��ʧ
		autoCell.life=0;
		//autoCell������
		{
			autoCell.sourcePos((float*)cp.pos2);
			autoCell.sourcePlay();
		}
	}

}
inline void collisionReact_cannonball_autoCell(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	Ccannonball&cannonball=*(Ccannonball*)cp.pmodel1;
	CautoCell&autoCell=*(CautoCell*)cp.pmodel2;
	//autoCell����ֵ����
	autoCell.life--;
	//autoCell������
	{
		autoCell.sourcePos((float*)cp.pos2);
		autoCell.sourcePlay();
	}
	//cannonball��ʧ
	cannonball.disapear();
	//���ڵ���ʧ������һ����ը
	layBurst(cp.pos1[X],cp.pos1[Y],cp.pos1[Z]);
}
inline void collisionReact_superburst_autoCell(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	Csuperburst&superburst=*(Csuperburst*)cp.pmodel1;
	CautoCell&autoCell=*(CautoCell*)cp.pmodel2;
	//autoCell����ֱֵ�Ӽ�Ϊ0
	autoCell.life=0;
	//autoCell������
	{
		autoCell.sourcePos((float*)cp.pos2);
		autoCell.sourcePlay();
	}
}
inline void collisionReact_pcell_amac(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	CmyCell*pcell=(CmyCell*)cp.pmodel1;
	Camac&amac=*(Camac*)cp.pmodel2;
	//myCell��������
	pcell->lifeplus(2);
	//���ų�С�������
	{
		alSource3f(source_eat_small,AL_POSITION,cp.pos1[X],cp.pos1[Y],cp.pos1[Z]);
		alSourcePlay(source_eat_small);
	}
	//amac��ʧ
	amac.disapear();
}

inline void collisionReact_pcell_ene(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	CmyCell*pcell=(CmyCell*)cp.pmodel1;
	Cene&ene=*(Cene*)cp.pmodel2;
	//myCell��������
	pcell->eneEatplus();
	//���ų�С�������
	{
		alSource3f(source_eat_small,AL_POSITION,cp.pos1[X],cp.pos1[Y],cp.pos1[Z]);
		alSourcePlay(source_eat_small);
	}
	//ene��ʧ
	ene.disapear();
}

inline void collisionReact_pcell_gene(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	CmyCell*pcell=(CmyCell*)cp.pmodel1;
	Cgene&gene=*(Cgene*)cp.pmodel2;
	//����gene��effectID����myCell���Ӻ��ּ���
	if(gene.effectID==0){
		//��Ծ
		pcell->setjumpable(true);				
	}else if(gene.effectID==1){
		//�޵�
		pcell->nomatch=true;
	}else if(gene.effectID==2){
		//����
		pcell->speedup();
	}else{
		//�ӷ�
		pcell->score+=2;
	}
	//���ų�С�������
	{
		alSource3f(source_eat_small,AL_POSITION,cp.pos1[X],cp.pos1[Y],cp.pos1[Z]);
		alSourcePlay(source_eat_small);
	}
	//gene��ʧ
	gene.disapear();
}



