
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
	bool getCollisionDeepth;//是否获得打入深度
	bool getCollisionNormals;//是否获得碰撞时二者的法向
	bool getCollisionDirs;//是否获得碰撞时二者朝向
	bool getCollisionPoses;//是否获得碰撞时二者位置
	bool getCollisionRs;//是否获得碰撞时二者的半径
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
	bool ignorY;//是否忽略Y坐标
	bool softBack;//是否软回退
	float k_softBack;//软回退系数，越小越软，为1是退化为硬回退，只有当softBack==true时有效
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
//						下面是与碰撞检测相关的函数
//--------------------------------------------------------------------------------------

bool collisionTest_BallBall(const float c1[4],const float R1,//ball1
	const float c2[4],const float R2,//ball2
	const CcollisionTestProperty&ctp,//碰撞属性
	CcollisionPair&cp)//返回的碰撞信息
//两个球之间的碰撞检测
//如果不碰撞，则n1,n2,d均无效
{
	float dis2=pow2(c1[X]-c2[X])+pow2(c1[Y]-c2[Y])+pow2(c1[Z]-c2[Z]);
	bool collision=(dis2<=pow2(R1+R2));
	//计算碰撞信息
	if(collision){//碰撞
		//计算打入深度
		if(ctp.getCollisionDeepth){
			float d=R1+R2-CarmSqrt(dis2);
			cp.setD(d);
		}
		//计算互斥向量
		if(ctp.getCollisionNormals){
			float n1[4],n2[4];
			sub((float(&)[4])(*c1),(float(&)[4])(*c2),(float(&)[4])(*n1));
			if(normalize(n1)==0){//如果两中心重合，反弹向量可任意规定，这里规定为(1,0,0,0)
				init4(n1,1,0,0,0);
			}
			neg(n1,n2);	
			cp.setNorm(n1,n2);
		}
		//记录位置
		if(ctp.getCollisionPoses){
			cp.setPos((float*)c1,(float*)c2);
			
		}
		return true;
	}else{//不碰撞
		return false;
	}			
}


inline void collision_model_modelList(Cmodel*pmodel,
	vector<Cmodel*>&pModelList,
	Ccgrid&grid,
	const CcollisionTestProperty&ctp,//碰撞属性
	const CcollisionReactProperty&crp,//碰撞反应属性
	void (&collisionReact)(const CcollisionPair&,const CcollisionReactProperty&)//碰撞函数
	)
{
	//获得碰撞列表
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
		int I=clist[i];//下标号
		Cmodel&model2=*pModelList[I];	
		if(model2.visible==false)continue;
		//检测autoCell与myCell的碰撞
		float pos2[4];//autoCell位置
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
			//处理碰撞反应
			collisionReact(cp,crp);
			break;//对于某个对象，检测到它的一个碰撞就行了，后面都这样处理

		}
	}
}
inline void collision_modelList_modelList(vector<Cmodel*>&pModelList1,
	vector<Cmodel*>&pModelList2,
	Ccgrid&grid2,
	const CcollisionTestProperty&ctp,//碰撞属性
	const CcollisionReactProperty&crp,//碰撞反应属性
	void (&collisionReact)(const CcollisionPair&,const CcollisionReactProperty&))//碰撞函数
{
	//首先判断pModelList1和pModelList2是否为同一对象
	bool List1EqList2=(&pModelList1==&pModelList2);//注意，应该用&pModelList1==&pModelList2...(1)
											  //而不要用pModelList1==pModelList2...(2)
	                                          //（1）表示判断pModelList1和pModelList2是否为同一个对象
											  //（2）表示判断pModelList1和pModelList2是否对应元素相等且元素数相同
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
		vector<int>&clist=grid2.clist;//碰撞列表
		
		//model1与clist中的autoCell检测碰撞
		int nc=(int)clist.size();
		for(int j=0;j<nc;j++){
			int J=clist[j];//下标号
			if(List1EqList2&&J<=i)continue;//如果pModelList1和pModelList2是同一个对象，
									    //则为了避免重复检测，跳过比model1下标号i小或相等的下标号
			                            //如果pModelList1和pModelList2不是同一个对象，
										//则不跳过。
			Cmodel&model2=*pModelList2[J];
			if(model2.visible==false)continue;
			//检测model1与model2的碰撞
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
				//处理碰撞反应
				collisionReact(cp,crp);
				break;//对于某个对象，检测到它的一个碰撞就行了，后面都这样处理

			}	
		}
	}


}

inline Cburst* layBurst(float x,float y,float z){//在指定位置放置一个爆炸
	//在炮弹消失处放置一个爆炸
	int ndiedburst=(int)pdiedburstList.size();
	if(ndiedburst!=0){//如果死亡爆炸列表中还有元素
		//从pdiedburstList中取一个加入到pburstList中
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
	//pmodel1回退一步
	float back[4];//pmodel1的回退向量
	mul(cp.d/2,cp.n1,back);//得到back
	//pmodel1回退
	cp.pmodel1->movloc_inFather(back[X],back[Y],back[Z]);
	//pmodel2转身
	float A2=getA((float*)cp.dir2,(float*)cp.n2);
	cp.pmodel2->rotloc010(A2);
	//pmodel2前进一步
	cp.pmodel2->movloc_001(-cp.d/2);

}
inline void collisionReact_Back_Back(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	float back[4];
	mul(cp.d/2,cp.n1,back);//得到back
	if(crp.ignorY)back[Y]=0;
	cp.pmodel1->movloc_inFather(back[X],back[Y],back[Z]);
	cp.pmodel2->movloc_inFather(-back[X],-back[Y],-back[Z]);
}

inline void collisionReact_Still_RollAndBack(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel2转身
	float A2=getA((float*)cp.dir2,(float*)cp.n2);
	cp.pmodel2->rotloc010(A2);
	//pmodel2前进一步
	cp.pmodel2->movloc_001(-cp.d/2);

}

inline void collisionReact_Back_Still(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel1回退
	float back[4];//回退向量
	mul(cp.d,cp.n1,back);
	if(crp.ignorY)back[Y]=0;//舍弃Y分量
	//按back回退
	cp.pmodel1->movloc_inFather(back[X],back[Y],back[Z]);
}
inline void collisionReact_BackSoft_Still(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel1回退
	float back[4];//回退向量
	mul(cp.d*crp.k_softBack,cp.n1,back);
	back[Y]=0;//舍弃Y分量
	//按back回退
	cp.pmodel1->movloc_inFather(back[X],back[Y],back[Z]);

}

inline void collisionReact_Still_BackSoft(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	//pmodel2回退
	float back[4];//回退向量
	mul(cp.d*crp.k_softBack,cp.n2,back);
	back[Y]=0;//舍弃Y分量
	//按back回退
	cp.pmodel2->movloc_inFather(back[X],back[Y],back[Z]);

}




//-------------------------------------------------------------------------------------------------
//             下面各函数写得非常垃圾，根本没有通用性，等有时间细考虑怎么改
//-------------------------------------------------------------------------------------------------



inline void collisionReact_pcell_autoCell(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	CmyCell&myCell=*(CmyCell*)cp.pmodel1;
	CautoCell&autoCell=*(CautoCell*)cp.pmodel2;
	if(myCell.nomatch==false){//如果myCell不是处于无敌状态
		//myCell生命减少
		myCell.lifeplus(-1);
		collisionReact_Back_RollAndBack(cp,crp);
	}else{//如果myCell处于无敌状态
		//autoCell消失
		autoCell.life=0;
		//autoCell叫声音
		{
			autoCell.sourcePos((float*)cp.pos2);
			autoCell.sourcePlay();
		}
	}

}
inline void collisionReact_cannonball_autoCell(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	Ccannonball&cannonball=*(Ccannonball*)cp.pmodel1;
	CautoCell&autoCell=*(CautoCell*)cp.pmodel2;
	//autoCell生命值减少
	autoCell.life--;
	//autoCell叫声音
	{
		autoCell.sourcePos((float*)cp.pos2);
		autoCell.sourcePlay();
	}
	//cannonball消失
	cannonball.disapear();
	//在炮弹消失处放置一个爆炸
	layBurst(cp.pos1[X],cp.pos1[Y],cp.pos1[Z]);
}
inline void collisionReact_superburst_autoCell(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	Csuperburst&superburst=*(Csuperburst*)cp.pmodel1;
	CautoCell&autoCell=*(CautoCell*)cp.pmodel2;
	//autoCell生命值直接减为0
	autoCell.life=0;
	//autoCell叫声音
	{
		autoCell.sourcePos((float*)cp.pos2);
		autoCell.sourcePlay();
	}
}
inline void collisionReact_pcell_amac(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	CmyCell*pcell=(CmyCell*)cp.pmodel1;
	Camac&amac=*(Camac*)cp.pmodel2;
	//myCell生命增加
	pcell->lifeplus(2);
	//播放吃小物件声音
	{
		alSource3f(source_eat_small,AL_POSITION,cp.pos1[X],cp.pos1[Y],cp.pos1[Z]);
		alSourcePlay(source_eat_small);
	}
	//amac消失
	amac.disapear();
}

inline void collisionReact_pcell_ene(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	CmyCell*pcell=(CmyCell*)cp.pmodel1;
	Cene&ene=*(Cene*)cp.pmodel2;
	//myCell能量增加
	pcell->eneEatplus();
	//播放吃小物件声音
	{
		alSource3f(source_eat_small,AL_POSITION,cp.pos1[X],cp.pos1[Y],cp.pos1[Z]);
		alSourcePlay(source_eat_small);
	}
	//ene消失
	ene.disapear();
}

inline void collisionReact_pcell_gene(const CcollisionPair&cp,const CcollisionReactProperty&crp){
	CmyCell*pcell=(CmyCell*)cp.pmodel1;
	Cgene&gene=*(Cgene*)cp.pmodel2;
	//根据gene的effectID决定myCell增加何种技能
	if(gene.effectID==0){
		//跳跃
		pcell->setjumpable(true);				
	}else if(gene.effectID==1){
		//无敌
		pcell->nomatch=true;
	}else if(gene.effectID==2){
		//加速
		pcell->speedup();
	}else{
		//加分
		pcell->score+=2;
	}
	//播放吃小物件声音
	{
		alSource3f(source_eat_small,AL_POSITION,cp.pos1[X],cp.pos1[Y],cp.pos1[Z]);
		alSourcePlay(source_eat_small);
	}
	//gene消失
	gene.disapear();
}



