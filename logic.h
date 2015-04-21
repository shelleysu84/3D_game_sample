
inline void generateCollsionGrids(){
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
	//----------------------------制作碰撞网格
	//制作autoCell碰撞网格
	autoCellCgrid.clearContent();
	autoCellCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	autoCellCgrid.fillContent(pautoCellList);
	//制作amac碰撞网格
	amacCgrid.clearContent();
	amacCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	amacCgrid.fillContent(pamacList);
	//制作ene碰撞网格
	eneCgrid.clearContent();
	eneCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	eneCgrid.fillContent(peneList);
	//制作gene碰撞网格
	geneCgrid.clearContent();
	geneCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	geneCgrid.fillContent(pgeneList);
	//制作tree碰撞网格
	treeCgrid.clearContent();
	treeCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	treeCgrid.fillContent(ptreeList);
}
inline void rebrithThings(long t){
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
	if(rand()%5==0){
		//增加气泡
		//随机生成坐标
		int x=mycellpos[X]+(rand()%(int)areaWidth_god-areaWidth_god/2)*0.5;
		int z=mycellpos[Z]+(rand()%(int)areaHeight_god-areaHeight_god/2)*0.5;
		//如果被地形覆盖，则不生成bubble，跳过
		float groundHorizon=ground.getH(x,z);
		if(groundHorizon<water.Horizon){//在水面，生成气泡
			Cbubble*pbubble=NULL;
			//看pdiedbubbleList中是否有元素
			if((int)pdiedbubbleList.size()!=0){
				//将pdiedbubbleList中最后一个元素取出来
				pbubble=pdiedbubbleList[(int)pdiedbubbleList.size()-1];
				pdiedbubbleList.pop_back();
				pbubble->reSet();
				pbubbleList.push_back(pbubble);
				pbubble->movloc(x,groundHorizon,z);
				bubbleRoot.addchild(pbubble);
				//添加声音
				if(rand()%2==0){//让声音不那么密集
					pbubble->sourcePos(x,groundHorizon,z);
					pbubble->sourcePlay();
				}
			}
			
		}
	}
	if(rand()%7==0){
		//增加液滴

		//生成坐标
		int x=mycellpos[X]+(rand()%(int)(areaWidth_god)-areaWidth_god/2)*0.6;
		int z=mycellpos[Z]+(rand()%(int)(areaHeight_god)-areaHeight_god/2)*0.6;
		//判断是在地面还是在水面
		float groundHorizon=ground.getH(x,z);
		if(groundHorizon<water.Horizon){//在水面，生成液滴
			Cdrop*pdrop=NULL;
			//看pdieddropList中是否有元素
			if((int)pdieddropList.size()!=0){
				//将pdieddropList中最后一个元素取出来
				pdrop=pdieddropList[(int)pdieddropList.size()-1];
				pdieddropList.pop_back();
				pdrop->reSet();
				pdropList.push_back(pdrop);
				Cdrop&drop=*pdrop;
				int colorIndex=rand()%(int)dropColorList.size();
				float R=dropColorList[colorIndex].R;
				float G=dropColorList[colorIndex].G;
				float B=dropColorList[colorIndex].B;
				drop.color[0]=R;
				drop.color[1]=G;
				drop.color[2]=B;
				drop.movloc(x,waterHorizon,z);
				//加入到dropRoot
				dropRoot.addchild(&drop);
			}

		}

	
	}
	//下面是一些补充较慢的东西，放在分频里进行
	if(rand()%10==0){

		//重生autoCell
		//如果pdiedautoCellList中有元素，则从中拿出一个重新利用
		int ndiedautoCell=(int)pdiedautoCellList.size();
		if(ndiedautoCell!=0){//有元素，循环利用
			//从pdiedautoCellList中拿出一个元素
			CautoCell*pautoCell=(CautoCell*)pdiedautoCellList[ndiedautoCell-1];
			pdiedautoCellList.pop_back();
			//将pautoCell重新初始化
			const float vlen=1.5/3/40;//死亡的细菌速度已发生了变化，所以速度需重新初始化
			int A=rand()*piDiv180;
			float vx=vlen*fastcos(A);
			float vz=vlen*fastsin(A);
			pautoCell->setV(vx,0,vz);
			int x=mycellpos[X]+rand()%(int)(width_mov)-(width_mov)/2;
			int z=mycellpos[Z]+rand()%(int)(height_mov)-(height_mov)/2;
			pautoCell->movloc_101(x,z);
			pautoCell->bflglr=false;//无鞭毛
			pautoCell->bshell=false;//无壳
			pautoCell->foot=false;//无脚（有脚的那个点，只是没有颜色）
			pautoCell->knee=false;//无膝（有膝的那个点，只是没有颜色）
			pautoCell->timecount=0;
			pautoCell->life=2;
			cellRoot.addchild(pautoCell);
			//着陆(必须放在加入到cellRoot之后)
			float groundHorizon=ground.getH(x,z);
			if(groundHorizon>water.Horizon){
				pautoCell->downToGround(2.0);
			}else{
				pautoCell->downToWaterSurface();

			}
			//将pautoCell加入到pautoCellList
			pautoCellList.push_back(pautoCell);
		}

	}
	if(rand()%10==0){
		//重生amac
		//从pdiedamacList中找一个
		int ndiedamac=(int)pdiedamacList.size();
		if(ndiedamac!=0){
			Camac*pamac=(Camac*)pdiedamacList[ndiedamac-1];
			pdiedamacList.pop_back();
			Camac&amac=*pamac;
			amac.setbdisapear(false);
			makeE(amac.RMmat);
			float vlen=1.0/40;//30/3;
			int A=rand()*piDiv180;
			float vx=vlen*fastcos(A);
			float vz=vlen*fastsin(A);
			amac.setV(vx,0,vz);
			int x=mycellpos[X]+rand()%(int)(width_mov)-(width_mov)/2;
			int z=mycellpos[Z]+rand()%(int)(height_mov)-(height_mov)/2;
			amac.movloc_101(x,z);
			amac.rotloc010(rand()%360);
			molecuRoot.addchild(&amac);
			pamacList.push_back(pamac);
			//着陆
			float groundHorizon=ground.getH(x,z);
			if(groundHorizon>water.Horizon){
				amac.downToGround(2.0);
			}else{
				amac.downToWaterSurface();
				
			}
		}

	}
	if(rand()%10==0){
		//重生gene
		int ndiedgene=(int)pdiedgeneList.size();
		if(ndiedgene!=0){
			Cgene*pgene=(Cgene*)pdiedgeneList[ndiedgene-1];
			pdiedgeneList.pop_back();
			Cgene&gene=*pgene;
			makeE(gene.RMmat);
			gene.setbdisapear(false);
			float vlen=1.0/40;//30/3;
			int A=rand()*piDiv180;
			float vx=vlen*fastcos(A);
			float vz=vlen*fastsin(A);
			gene.setV(vx,0,vz);
			int x=mycellpos[X]+rand()%(int)areaWidth_god-areaWidth_god/2;
			int z=mycellpos[Z]+rand()%(int)areaHeight_god-areaHeight_god/2;
			gene.movloc_101(x,z);
			molecuRoot.addchild(&gene);
			pgeneList.push_back(&gene);
	
			//着陆
			float groundHorizon=ground.getH(x,z);
			if(groundHorizon>water.Horizon){
				gene.downToGround(2.0);
			}else{
				gene.downToWaterSurface();
				
			}
			
		}
	}
	if(rand()%10==0){
		//重生ene
		int ndiedene=(int)pdiedeneList.size();
		if(ndiedene!=0){
			Cene*pene=(Cene*)pdiedeneList[ndiedene-1];
			pdiedeneList.pop_back();
			Cene&ene=*pene;
			makeE(ene.RMmat);
			ene.setbdisapear(false);
			float vlen=1.0/40;//30/3;
			int A=rand()*piDiv180;
			float vx=vlen*fastcos(A);
			float vz=vlen*fastsin(A);
			ene.setV(vx,0,vz);
			int x=mycellpos[X]+rand()%(int)(width_mov)-(width_mov)/2;
			int z=mycellpos[Z]+rand()%(int)(height_mov)-(height_mov)/2;
			ene.movloc_101(x,z);
			ene.rotloc010(rand()%360);
			molecuRoot.addchild(&ene);
			peneList.push_back(&ene);
			//着陆
			float groundHorizon=ground.getH(x,z);
			if(groundHorizon>water.Horizon){
				ene.downToGround(2.0);
			}else{
				ene.downToWaterSurface();
				
			}
		}	
		
	}

}

inline void movInBond(float width_mov,float height_mov,long dt,long t){
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
	//将出界的modelx移到界内
	{
		float width_mov_modelx=width_mov*0.9;
		float height_mov_modelx=height_mov*0.9;
		float pos_modelx[4];
		modelx.getPosInFather(pos_modelx);
		bool moved=false;
		if(pos_modelx[X]<mycellpos[X]-(width_mov_modelx)/2){
			modelx.movloc_inFather(width_mov_modelx,0,0);
			moved=true;
		}
		if(pos_modelx[Z]<mycellpos[Z]-(height_mov_modelx)/2){
			modelx.movloc_inFather(0,0,height_mov_modelx);
			moved=true;
		}
		if(pos_modelx[X]>mycellpos[X]+(width_mov_modelx)/2){
			modelx.movloc_inFather(-width_mov_modelx,0,0);
			moved=true;
		}
		if(pos_modelx[Z]>mycellpos[Z]+(height_mov_modelx)/2){
			modelx.movloc_inFather(0,0,-height_mov_modelx);
			moved=true;
		}
		if(moved)modelx.downToGround_soft();
		

	}
	//将出界的tree移到界内
	{
		int ntree=(int)ptreeList.size();
		for(int i=0;i<ntree;i++){
			Ctree&tree=*(Ctree*)ptreeList[i];
			float treepos[4];
			tree.getPosInFather(treepos);
			float x=treepos[X];
			float z=treepos[Z];
			bool moved=false;
			if(x<mycellpos[X]-(width_mov)/2){
				tree.movloc_inFather(width_mov,0,0);
				moved=true;
			}
			if(z<mycellpos[Z]-(height_mov)/2){
				tree.movloc_inFather(0,0,height_mov);
				moved=true;
			}
			if(x>mycellpos[X]+(width_mov)/2){
				tree.movloc_inFather(-width_mov,0,0);
				moved=true;
			}
			if(z>mycellpos[Z]+(height_mov)/2){
				tree.movloc_inFather(0,0,-height_mov);
				moved=true;
			}
			if(moved){
				float treenewpos[4];
				tree.getPosInFather(treenewpos);
				//着陆
				float groundHorizon=ground.getH(treenewpos[X],treenewpos[Z]);
				if(groundHorizon>water.Horizon){
					tree.downToGround(0,false);
				}else{
					tree.downToWaterSurface();
					
				}
			}
			
		}
	}

	int width_mov_molecule=width_mov*0.6;
	int height_mov_molecule=height_mov*0.6;

	
	//将出界的amac移到界内
	{
		int namac=(int)pamacList.size();
		for(int i=0;i<namac;i++){
			Camac&amac=*(Camac*)pamacList[i];
			float amacpos[4];
			amac.getPosInFather(amacpos);
			float x=amacpos[X];
			float z=amacpos[Z];
			bool moved=false;
			if(x<mycellpos[X]-(width_mov_molecule)/2){
				amac.movloc_inFather(width_mov_molecule,0,0);
				moved=true;
			}
			if(z<mycellpos[Z]-(height_mov_molecule)/2){
				amac.movloc_inFather(0,0,height_mov_molecule);
				moved=true;
			}
			if(x>mycellpos[X]+(width_mov_molecule)/2){
				amac.movloc_inFather(-width_mov_molecule,0,0);
				moved=true;
			}
			if(z>mycellpos[Z]+(height_mov_molecule)/2){
				amac.movloc_inFather(0,0,-height_mov_molecule);
				moved=true;
			}
			if(moved){
				float amacnewpos[4];
				amac.getPosInFather(amacnewpos);
				//着陆
				float groundHorizon=ground.getH(amacnewpos[X],amacnewpos[Z]);
				if(groundHorizon>water.Horizon){
					amac.downToGround(2.0);
				}else{
					amac.downToWaterSurface();
					
				}
			}
			
		}
	}
	//amac移动
	{
		int namac=(int)pamacList.size();
		for(int i=0;i<namac;i++){
			Camac&amac=*(Camac*)pamacList[i];
			if(amac.visible==false)continue;
			if(amac.isOnWater()){
				amac.floatOnWater(dt);
				amac.rotloc010(0.6*dt);
				//判断与house的碰撞
				float amacpos[4];
				amac.getPosInFather(amacpos);
				float e_back[4];
				float d=house.collisionTestWithWall_singlePoint(amacpos,amac.Rc,e_back);
				if(d>0){//与墙壁发生碰撞
					//移动
					e_back[Y]=0;//舍掉Y分量（防止分子飞天或潜水）
					float back[4];
					mul(d,e_back,back);
					amac.movloc_inFather(back[X],back[Y],back[Z]);
					//改变速度
					float *v=amac.v;//入射速度
					//反射速度v2=v-2*dot(v,e_back)*e_back
					//（下面令k=-2*dot(v,e_back)，u=k*e_back，则v2=v+u）
					float v2[4];
					float k=-2*dot(v,e_back);
					float u[4];
					mul(k,e_back,u);
					add(v,u,v2);//得到v2
					//用v2改写v
					veccopy(v2,v);
				}
			}	
			
		}
	}
	
	//将出界的ene移到界内
	{
		int nene=(int)peneList.size();
		for(int i=0;i<nene;i++){
			Cene&ene=*(Cene*)peneList[i];
			float enepos[4];
			ene.getPosInFather(enepos);
			float x=enepos[X];
			float z=enepos[Z];
			bool moved=false;
			if(x<mycellpos[X]-(width_mov_molecule)/2){
				ene.movloc_inFather(width_mov_molecule,0,0);
				moved=true;
			}
			if(z<mycellpos[Z]-(height_mov_molecule)/2){
				ene.movloc_inFather(0,0,height_mov_molecule);
				moved=true;
			}
			if(x>mycellpos[X]+(width_mov_molecule)/2){
				ene.movloc_inFather(-width_mov_molecule,0,0);
				moved=true;
			}
			if(z>mycellpos[Z]+(height_mov_molecule)/2){
				ene.movloc_inFather(0,0,-height_mov_molecule);
				moved=true;
			}
			if(moved){
				float enenewpos[4];
				ene.getPosInFather(enenewpos);
				//着陆
				float groundHorizon=ground.getH(enenewpos[X],enenewpos[Z]);
				if(groundHorizon>water.Horizon){
					ene.downToGround(2.0);
				}else{
					ene.downToWaterSurface();		
				}
			}
			
		}
	}
	//ene移动
	{
		int nene=(int)peneList.size();
		for(int i=0;i<nene;i++){
			Cene&ene=*(Cene*)peneList[i];
			if(ene.visible==false)continue;
			if(ene.isOnWater()){
				ene.floatOnWater(dt);
				ene.rotloc010(0.9*dt);
				//判断与house的碰撞
				float enepos[4];
				ene.getPosInFather(enepos);
				float e_back[4];
				float d=house.collisionTestWithWall_singlePoint(enepos,ene.Rc,e_back);
				if(d>0){//与墙壁发生碰撞
					//移动
					e_back[Y]=0;//舍掉Y分量（防止分子飞天或潜水）
					float back[4];
					mul(d,e_back,back);
					ene.movloc_inFather(back[X],back[Y],back[Z]);
					//改变速度
					float *v=ene.v;//入射速度
					//反射速度v2=v-2*dot(v,e_back)*e_back
					//（下面令k=-2*dot(v,e_back)，u=k*e_back，则v2=v+u）
					float v2[4];
					float k=-2*dot(v,e_back);
					float u[4];
					mul(k,e_back,u);
					add(v,u,v2);//得到v2
					//用v2改写v
					veccopy(v2,v);
				}
			}
		}
	}
	
	//将出界的gene移到界内
	{
		int ngene=(int)pgeneList.size();
		for(int i=0;i<ngene;i++){
			Cgene&gene=*(Cgene*)pgeneList[i];
			float genepos[4];
			gene.getPosInFather(genepos);
			float x=genepos[X];
			float z=genepos[Z];
			bool moved=false;
			if(x<mycellpos[X]-(width_mov_molecule)/2){
				gene.movloc_inFather(width_mov_molecule,0,0);
				moved=true;
			}
			if(z<mycellpos[Z]-(height_mov_molecule)/2){
				gene.movloc_inFather(0,0,height_mov_molecule);
				moved=true;
			}
			if(x>mycellpos[X]+(width_mov_molecule)/2){
				gene.movloc_inFather(-width_mov_molecule,0,0);
				moved=true;
			}
			if(z>mycellpos[Z]+(height_mov_molecule)/2){
				gene.movloc_inFather(0,0,-height_mov_molecule);
				moved=true;
			}
			if(moved){
				float genenewpos[4];
				gene.getPosInFather(genenewpos);
				//着陆
				float groundHorizon=ground.getH(genenewpos[X],genenewpos[Z]);
				if(groundHorizon>water.Horizon){
					gene.downToGround(2.0);
				}else{
					gene.downToWaterSurface();
					
				}
			}
			
		}
	}
	//gene移动
	{
		int ngene=(int)pgeneList.size();
		for(int i=0;i<ngene;i++){
			Cgene&gene=*(Cgene*)pgeneList[i];
			if(gene.visible==false)continue;
			if(gene.isOnWater()){
				gene.floatOnWater(dt);
				//判断与house的碰撞
				float genepos[4];
				gene.getPosInFather(genepos);
				float e_back[4];
				float d=house.collisionTestWithWall_singlePoint(genepos,gene.Rc,e_back);
				if(d>0){//与墙壁发生碰撞
					//移动
					e_back[Y]=0;//舍掉Y分量（防止分子飞天或潜水）
					float back[4];
					mul(d,e_back,back);
					gene.movloc_inFather(back[X],back[Y],back[Z]);
					//改变速度
					float *v=gene.v;//入射速度
					//反射速度v2=v-2*dot(v,e_back)*e_back
					//（下面令k=-2*dot(v,e_back)，u=k*e_back，则v2=v+u）
					float v2[4];
					float k=-2*dot(v,e_back);
					float u[4];
					mul(k,e_back,u);
					add(v,u,v2);//得到v2
					//用v2改写v
					veccopy(v2,v);
				}
			}
		}
	}
	
	//将出界的autoCell移到界内
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			float autoCellpos[4];
			autoCell.getPosInFather(autoCellpos);
			float x=autoCellpos[X];
			float z=autoCellpos[Z];
			bool moved=false;
			if(x<mycellpos[X]-(width_mov)/2){
				autoCell.movloc_inFather(width_mov,0,0);
				moved=true;
			}
			if(z<mycellpos[Z]-(height_mov)/2){
				autoCell.movloc_inFather(0,0,height_mov);
				moved=true;
			}
			if(x>mycellpos[X]+(width_mov)/2){
				autoCell.movloc_inFather(-width_mov,0,0);
				moved=true;
			}
			if(z>mycellpos[Z]+(height_mov)/2){
				autoCell.movloc_inFather(0,0,-height_mov);
				moved=true;
			}
		}
	}
	
	

}
inline void ridDiedThings(){
		
	//-----------由于列表在循环中长度发生变化，所以若将上界提前固化，则循环体内应加n--
	//将life减为0的autoCell删除
	{
		int nautoCell=(int)pautoCellList.size();
		int count=0;
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			if(autoCell.life<=0){
				//主角获得分数
				pcell->score+=10;
				//从场景树上摘下来
				cellRoot.ridchild(&autoCell);
				//加入到死亡敌菌列表
				pdiedautoCellList.push_back(&autoCell);
				count++;
			}else{
				if(count!=0)pautoCellList[i-count]=&autoCell;
			}
		}
		pautoCellList.resize(nautoCell-count);
	}

	//将标记为disapear的炮弹删除
	{
		int ncannonball=(int)pcell->pcannonballList.size();
		int count=0;
		for(int i=0;i<ncannonball;i++){
			Ccannonball&cannonball=*(Ccannonball*)pcell->pcannonballList[i];
			if(cannonball.isdisapear()){
				CweaponMover::cannonballRoot.ridchild(&cannonball);//将炮弹从cannonballRoot上取下
				//加入到pcell->pdiedcannonballList
				pcell->pdiedcannonballList.push_back(&cannonball);
				count++;
			}else{
				if(count!=0)pcell->pcannonballList[i-count]=&cannonball;
			}
		}
		pcell->pcannonballList.resize(ncannonball-count);
	}
	//将标记为disapear的amac删除
	{
		int namac=(int)pamacList.size();
		int count=0;
		for(int i=0;i<namac;i++){
			Camac&amac=*(Camac*)pamacList[i];
			if(amac.isdisapear()){
				//从molecuRoot上摘下来
				molecuRoot.ridchild(&amac);
				//加到pdiedamacList中
				pdiedamacList.push_back(&amac);
				count++;
			}else{
				if(count!=0)pamacList[i-count]=&amac;
				
			}
		}
		pamacList.resize(namac-count);
	}
	//将标记为disapear的ene删除
	{
		int nene=(int)peneList.size();
		int count=0;
		for(int i=0;i<nene;i++){
			Cene&ene=*(Cene*)peneList[i];
			if(ene.isdisapear()){
				//从molecuRoot上摘下来
				molecuRoot.ridchild(&ene);
				//加入到pdiedeneList
				pdiedeneList.push_back(&ene);
				count++;
			}else{
				if(count!=0)peneList[i-count]=&ene;
			}
		}
		peneList.resize(nene-count);
	}
	//将标记为disapear的gene删除
	{
		int ngene=(int)pgeneList.size();
		int count=0;
		for(int i=0;i<ngene;i++){
			Cgene&gene=*(Cgene*)pgeneList[i];
			if(gene.isdisapear()){
				//从molecuRoot上摘下来
				molecuRoot.ridchild(&gene);
				//加入到pdiedgeneList
				pdiedgeneList.push_back(&gene);
				count++;
			}else{
				if(count!=0)pgeneList[i-count]=&gene;
			}
		}
		pgeneList.resize(ngene-count);
	}
	//将标记为disapear的protein删除
	{
		int nprotein=(int)pproteinList.size();
		int count=0;
		for(int i=0;i<nprotein;i++){
			Cprotein&protein=*(Cprotein*)pproteinList[i];
			if(protein.isdisapear()){
				//从proteinRoot上摘下来
				proteinRoot.ridchild(&protein);
				//加入到pdiedproteinList
				pdiedproteinList.push_back(&protein);
				count++;
			}else{
				if(count!=0)pproteinList[i-count]=&protein;
			}
		}
		pproteinList.resize(nprotein-count);
	}
	
	//清除已结束的气泡
	{
		int nbubble=(int)pbubbleList.size();
		int count=0;
		for(int i=0;i<nbubble;i++){
			Cbubble&bubble=*pbubbleList[i];
			if(bubble.isdied){
				//从场景树上摘下来
				bubbleRoot.ridchild(&bubble);
				//加入到pdiedbubbleList
				pdiedbubbleList.push_back(&bubble);
				count++;
			}else{
				if(count!=0)pbubbleList[i-count]=&bubble;
			}
		}
		pbubbleList.resize(nbubble-count);
	}
	//清除已结束的液滴
	{
		int ndrop=(int)pdropList.size();
		int count=0;
		for(int i=0;i<ndrop;i++){
			Cdrop&drop=*pdropList[i];
			if(drop.isdied){
				//从dropRoot上摘下来
				dropRoot.ridchild(&drop);
				//加入到pdieddropList
				pdieddropList.push_back(&drop);
				count++;
			}else{
				if(count!=0)pdropList[i-count]=&drop;
			}
		}
		pdropList.resize(ndrop-count);
	}
	
	//将disapear的bigcannon加入pdiedbigcannonList列表
	{
		int nbigcannon=(int)pbigcannonList.size();
		int count=0;
		for(int i=0;i<nbigcannon;i++){
			Cbigcannon&bigcannon=*pbigcannonList[i];
			if(bigcannon.getbdisapear()){
				//从bigcannonRoot上摘下来
				bigcannonRoot.ridchild(&bigcannon);
				//加入到pdiedbigcannonList中
				pdiedbigcannonList.push_back(&bigcannon);
				count++;
			}else{
				if(count!=0)pbigcannonList[i-count]=&bigcannon;
			}
		}
		pbigcannonList.resize(nbigcannon-count);
	}
	
	//将已结束的superburst删除
	{
		int nsuperburst=(int)psuperburstList.size();
		int count=0;
		for(int i=0;i<nsuperburst;i++){
			Csuperburst&superburst=*(Csuperburst*)psuperburstList[i];
			if(superburst.isdied){
				//从superburstRoot上摘下来
				superburstRoot.ridchild(&superburst);
				//加入到pdiedsuperburstList
				pdiedsuperburstList.push_back(&superburst);
				count++;
			}else{
				if(count!=0)psuperburstList[i-count]=&superburst;
			}
		}
		psuperburstList.resize(nsuperburst-count);
	}
	//将已结束的burst加入pdiedburstList列表
	{
		int nburst=(int)pburstList.size();
		int count=0;
		for(int i=0;i<nburst;i++){
			Cburst&burst=*pburstList[i];
			if(burst.isdied){
				//从场景树上摘下来
				burstRoot.ridchild(&burst);
				//加入到pdiedburstList中
				pdiedburstList.push_back(&burst);
				count++;
			}else{
				if(count!=0)pburstList[i-count]=&burst;
			}
		}
		pburstList.resize(nburst-count);
	}

}
inline void logic_collsions(long dt,long t){
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
	//------------------------------碰撞检测(碰撞检测放在前面，防止各pList失效)------------------------------------------
	//autoCell之间的碰撞，使用网格机制
	{
		CcollisionTestProperty ctp;ctp.set(true,true,true);
		CcollisionReactProperty crp;crp.set(true,false,1);
		collision_modelList_modelList(pautoCellList,pautoCellList,autoCellCgrid,ctp,crp,
			collisionReact_BothRollAndBack);
	}	
	
	//autoCell与tree碰撞，使用网格机制
	{
		CcollisionTestProperty ctp;ctp.set(true,true,true);
		CcollisionReactProperty crp;crp.set(true,false,1);
		collision_modelList_modelList(pautoCellList,ptreeList,treeCgrid,ctp,crp,
			collisionReact_Back_Still);
	}
	
	//cannonball与autoCell碰撞，使用网格机制
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_modelList_modelList(pcell->pcannonballList,pautoCellList,autoCellCgrid,ctp,crp,
			collisionReact_cannonball_autoCell);
	
	}
	//superburst与autocell碰撞，使用网格机制
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_modelList_modelList(psuperburstList,pautoCellList,autoCellCgrid,ctp,crp,
			collisionReact_superburst_autoCell);

	}
	//cannonball与地面碰撞
	{
		int ncannon=(int)pcell->pcannonballList.size();
		for(int i=0;i<ncannon;i++){
			Ccannonball&cannonball=*(Ccannonball*)pcell->pcannonballList[i];
			float ballpos[4];
			cannonball.getPosInFather(ballpos);
			float groundH=ground.getH(ballpos[X],ballpos[Z]);
			if(ballpos[Y]<=groundH){
				//cannonball消失
				cannonball.disapear();
				//在炮弹消失处放置一个爆炸
				layBurst(ballpos[X],groundH,ballpos[Z]);
			}
		}
	}
	//*pcell与autoCell碰撞，使用网格机制
	{
		CcollisionTestProperty ctp;ctp.set(true,true,true);
		CcollisionReactProperty crp;crp.set(true,false,1);
		collision_model_modelList(pcell,pautoCellList,autoCellCgrid,ctp,crp,
			collisionReact_pcell_autoCell);
	}
	//*pcell与tree碰撞，使用网格机制
	{
		CcollisionTestProperty ctp;ctp.set(true,true,false);
		CcollisionReactProperty crp;crp.set(true,true,0.25);
		collision_model_modelList(pcell,ptreeList,treeCgrid,ctp,crp,
			collisionReact_Back_Still);
	}
	//*pcell与amac碰撞，使用网格机制
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_model_modelList(pcell,pamacList,amacCgrid,ctp,crp,
			collisionReact_pcell_amac);
	}
	//*pcell与ene碰撞
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_model_modelList(pcell,peneList,eneCgrid,ctp,crp,
			collisionReact_pcell_ene);
	}
	//*pcell与gene碰撞
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_model_modelList(pcell,pgeneList,geneCgrid,ctp,crp,
			collisionReact_pcell_gene);

	}
	

	//cannonball与house碰撞
	{
		int ncannonball=(int)pcell->pcannonballList.size();
		for(int i=0;i<ncannonball;i++){
			Ccannonball&cannonball=*(Ccannonball*)pcell->pcannonballList[i];
			//当前炮弹位置
			float ballpos[4];
			cannonball.getPosInFather(ballpos);
			//前一时刻炮弹位置
			float *ballposf=cannonball.posf;
			//判断线段ballposf--ballpos是否与house相交
			float p[4];//交点坐标
			if(house.collisionTest_lineSeg(ballposf,ballpos,p)){
				//cannonball消失
				cannonball.disapear();
				//在炮弹消失处放置一个爆炸
				Cburst*pburst=layBurst(p[X],p[Y],p[Z]);
				if(pburst!=NULL){
					//pburst音效，打在墙壁上的声音
					pburst->sourcePos(p);
					pburst->sourcePlay();
				}
			}
		}
	}
	//modelx与pcell碰撞
	{
		if(modelx.visible){
			CcollisionTestProperty ctp;ctp.set(true,true,false);
			CcollisionReactProperty crp;crp.set(true,false,1);
			float *pos1=mycellpos;
			float pos2[4];
			modelx.getPosInFather(pos2);
			float R1=pcell->Rc;
			float R2=modelx.Rc;
			CcollisionPair cp;
			cp.setPmodel(pcell,&modelx);
			bool collision=collisionTest_BallBall(pos1,R1,pos2,R2,ctp,cp);
			if(collision){//碰撞
				collisionReact_Back_Back(cp,crp);
				//声音
				{
					bool allstopped;//是否两个声音都停止了
					ALint state1;
					alGetSourcei(source_modelx_hellow_i_m_beavis, AL_SOURCE_STATE,&state1);
					ALint state2;
					alGetSourcei(source_modelx_what_are_you_doing, AL_SOURCE_STATE,&state2);
					if((state1==AL_STOPPED||state1==AL_INITIAL)&&(state2==AL_STOPPED||state2==AL_INITIAL)){
						allstopped=true;
					}else{
						allstopped=false;
					}
					if(allstopped){
						//根据modelx与pcell的朝向决定播放哪个声音
						float dir_modelx[4];
						modelx.getDir(dir_modelx);
						if(dot(cp.n1,dir_modelx)>=0){//迎面走来
							//播放hellow, i'm beavis
							alSource3f(source_modelx_hellow_i_m_beavis,AL_POSITION,pos2[X],pos2[Y],pos2[Z]);
							alSourcePlay(source_modelx_hellow_i_m_beavis);
						}else{//追尾
							//播放what are you doing
							alSource3f(source_modelx_what_are_you_doing,AL_POSITION,pos2[X],pos2[Y],pos2[Z]);
							alSourcePlay(source_modelx_what_are_you_doing);
						}	
					}
				}
				
			}
		}	
	}
	//modelx与modelx2碰撞
	{
		if(modelx.visible){
			CcollisionTestProperty ctp;ctp.set(true,true,false);
			CcollisionReactProperty crp;crp.set(true,false,1);
			float pos1[4];
			modelx.getPosInFather(pos1);
			float pos2[4];
			modelx2.getPosInFather(pos2);
			float R1=modelx.Rc;
			float R2=modelx2.Rc;
			CcollisionPair cp;
			cp.setPmodel(&modelx,&modelx2);
			bool collision=collisionTest_BallBall(pos1,R1,pos2,R2,ctp,cp);
			if(collision){//碰撞
				collisionReact_Back_Still(cp,crp);
			}
		}	
	}
	//modelx2与pcell碰撞
	{
		if(modelx2.visible){
			float *pos1=mycellpos;
			float pos2[4];
			modelx2.getPosInFather(pos2);
			float R1=pcell->Rc;
			float R2=modelx2.Rc;
			float dis2=pow2(pos1[X]-pos2[X])+pow2(pos1[Y]-pos2[Y])+pow2(pos1[Z]-pos2[Z]);
			if(dis2<=pow2(R1+R2)){//碰撞
				
				float d=fabs((R1+R2)-CarmSqrt(dis2));//碰撞的法向深度
				                                     //加上fabs是为了避免d为负值，之所以会产生d为负值的情况，是因为CarmSqrt不精确
				//pcell回退一步
				//求回退向量
				float n1[4];//pcell的反射向量
				sub((float(&)[4])(*pos1),(float(&)[4])(*pos2),n1);
				n1[Y]=0;
				float back[4];//pcell的回退向量
				if(normalize(n1)!=0){
					mul(d,n1,back);//得到back
					//pcell回退
					pcell->movloc_inFather(back[X],back[Y],back[Z]);
				}
			}
		}	
	}
	//modelx与autoCell碰撞，使用网格机制
	{
		if(modelx.visible){
			CcollisionTestProperty ctp;ctp.set(true,true,true);
			CcollisionReactProperty crp;crp.set(true,false,1);
			collision_model_modelList(&modelx,pautoCellList,autoCellCgrid,ctp,crp,
				collisionReact_Back_RollAndBack);
		}
	}
	//modelx2与autoCell碰撞，使用网格机制
	{
		if(modelx2.visible){
			CcollisionTestProperty ctp;ctp.set(true,true,true);
			CcollisionReactProperty crp;crp.set(true,false,1);
			collision_model_modelList(&modelx2,pautoCellList,autoCellCgrid,ctp,crp,
				collisionReact_Still_RollAndBack);
		}
	}
	
	
	//metalBall与pcell碰撞
	{
		if(metalBall.visible){
			float *pos1=mycellpos;
			float pos2[4];
			metalBall.getPosInFather(pos2);
			float R1=pcell->Rc;
			float R2=metalBall.Rc;
			float dis2=pow2(pos1[X]-pos2[X])+pow2(pos1[Y]-pos2[Y])+pow2(pos1[Z]-pos2[Z]);
			if(dis2<=pow2(R1+R2)){//碰撞
				
				float d=fabs((R1+R2)-CarmSqrt(dis2));//碰撞的法向深度
				                                     //加上fabs是为了避免d为负值，之所以会产生d为负值的情况，是因为CarmSqrt不精确
				//pcell回退一步
				//求回退向量
				float n1[4];//pcell的反射向量
				sub((float(&)[4])(*pos1),pos2,n1);
				n1[Y]=0;
				float back[4];//pcell的回退向量
				if(normalize(n1)!=0){
					mul(d,n1,back);//得到back
					//pcell回退
					pcell->movloc_inFather(back[X],back[Y],back[Z]);
				}
			}
		}	
	}
	
	
	//pcell与protein的碰撞检测
	{
		int n=(int)pproteinList.size();
		for(int i=0;i<n;i++){
			Cprotein&protein=*pproteinList[i];
			if(protein.visible){
				float R1=pcell->Rc;
				float R2=protein.Rc;
				float *pos1=mycellpos;
				float pos2[4];//protein的坐标
				protein.getPosInFather(pos2);
				if(pow2(pos1[X]-pos2[X])+pow2(pos1[Y]-pos2[Y])+pow2(pos1[Z]-pos2[Z])<pow2(R1+R2)
					&&fabs(pos1[Y]-pos2[Y])<R2+R1/6){//碰撞
						//protein消失
						protein.disapear();
						NproteinEaten++;
						//检查是否所有的protein都已吃完
						if(NproteinEaten==NproteinMax){
							missionDone=true;
						}
						//播放mission_done音效
						alSource3f(source_eatProtein,AL_POSITION,pos1[X],pos1[Y],pos1[Z]);//用pos1而不用pos2
						alSourcePlay(source_eatProtein);
						if(missionDone){//如果missionDone，还要同时播放祝贺声音
							alSource3f(source_mission_done,AL_POSITION,pos1[X],pos1[Y],pos1[Z]);//用pos1而不用pos2
							alSourcePlay(source_mission_done);
						}
				}
			}
		}
	}

}


void audit_superburst(){
	cout<<"----"<<endl;
	cout<<(int)psuperburstList.size()<<" "<<pdiedsuperburstList.size()<<" ";
	cout<<(int)psuperburstList.size()+(int)pdiedsuperburstList.size();
	cout<<" ";
	cout<<Nsuperburst;
	cout<<" ";
	cout<<(int)superburstRoot.p.size();
	cout<<endl;
}
void audit_bigcannon(){
	cout<<"----"<<endl;
	cout<<(int)pbigcannonList.size()+(int)pdiedbigcannonList.size();
	cout<<" ";
	cout<<Nbigcannon;
	cout<<" ";
	cout<<(int)bigcannonRoot.p.size();
	cout<<endl;
}
void audit_burst(){
	cout<<"----"<<endl;
	cout<<(int)pburstList.size()+(int)pdiedburstList.size();
	cout<<" ";
	cout<<Nburst;
	cout<<" ";
	cout<<(int)burstRoot.p.size();
	cout<<endl;
}
void audit_drop(){
	cout<<"----"<<endl;
	cout<<(int)pdropList.size()+(int)pdieddropList.size();
	cout<<" ";
	cout<<Ndrop;
	cout<<" ";
	cout<<(int)dropRoot.p.size();
	cout<<endl;
}
void audit_bubble(){
	cout<<"----"<<endl;
	cout<<(int)pbubbleList.size()+(int)pdiedbubbleList.size();
	cout<<" ";
	cout<<Nbubble;
	cout<<" ";
	cout<<(int)bubbleRoot.p.size();
	cout<<endl;
}
void audit_autoCell(){
	cout<<"----"<<endl;
	cout<<(int)pautoCellList.size()+(int)pdiedautoCellList.size();
	cout<<" ";
	cout<<NautoCell;
	cout<<" ";
	cout<<"cellRoot:"<<(int)cellRoot.p.size();
	cout<<endl;
}
void audit_molecu(){
	cout<<"----"<<endl;
	cout<<(int)pamacList.size()+(int)pdiedamacList.size();
	cout<<" ";
	cout<<Namac;
	cout<<";";
	cout<<(int)peneList.size()+(int)pdiedeneList.size();
	cout<<" ";
	cout<<Nene;
	cout<<";";
	cout<<(int)pgeneList.size()+(int)pdiedgeneList.size();
	cout<<" ";
	cout<<Ngene;
	cout<<";";
	cout<<"molecuRoot:"<<(int)molecuRoot.p.size();
	cout<<endl;
}
inline void updateLogic(long dt,long t){
	////////////////////  对各列表进行审计  ///////////////////////
	{
	//	audit_superburst();
	//	audit_bigcannon();
	//	audit_burst();
	//	audit_drop();
	//	audit_bubble();
	//	audit_autoCell();
	//	audit_molecu();
		

	}
	//////////////////////////////////////////////////////////////
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
//	printv(mycellpos);
	//碰撞检测
	{
		//生成碰撞网格
		generateCollsionGrids();
		//碰撞检测逻辑
		logic_collsions(dt,t);
	}
	//检查各炮弹是否己达到射程
	{
		int ncanonball=(int)pcell->pcannonballList.size();
		for(int i=0;i<ncanonball;i++){
			Ccannonball&cannonball=*(Ccannonball*)pcell->pcannonballList[i];
			//如果距离pcell较远，则消失
			float cannonballpos[4];
			cannonball.getPosInFather(cannonballpos);
			float vec[4];
			sub(mycellpos,(float(&)[4])(*cannonballpos),vec);
			if(getLen_2(vec)>=200.0*200.0){
				cannonball.disapear();
			}
		}
	}
	//--------------------移动----------------------------------
	//modelx运动
	{
		//但不管可不可见，都要移动并保持落到地面上
		if(modelx.visible){
			modelx.autoMov(house,dt,t);
		}else{
			modelx.autoMov_whenVisibleIsFalse(dt,t);
		}
		modelx.downToGround_soft();
		//不管可见与否，都是进行动画，反正动动画行进不需要任何开销，只是时间++而已
		modelx.modelfbx.animationAdvance_rollback(1*dt);
		
	//	cout<<"modelx.visible:"<<modelx.visible<<endl;
	//	cout<<"modelx.visible_reflect:"<<modelx.visible_reflect<<endl;

	}
	//modelx2
	{
	//	modelx2.downToGround_soft();
		//不管可见与否都播动画，反正播动画不需任何开销，仅是时间++而已
		modelx2.modelfbx.animationAdvance_rollback(1*dt);

		//求pcell到modelx2的距离平方
		float modelx2pos[4];
		modelx2.getPosInFather(modelx2pos);
		float d2=pow2(mycellpos[X]-modelx2pos[X])+pow2(mycellpos[Y]-modelx2pos[Y])+pow2(mycellpos[Z]-modelx2pos[Z]);
		const float peakValue=6;
		const float maxDis=200.0;
		const float k=-peakValue/maxDis;
		float d=fabs(CarmSqrt(d2));
		float currentValue=max(0,peakValue+k*d);
		alSourcef(source_music, AL_GAIN,currentValue);
		if(currentValue>0){//有音
			ALint state;
			alGetSourcei(source_music, AL_SOURCE_STATE,&state);
			switch(state){//AL_SOURCE_STATE只有这四种
			case AL_INITIAL:
			case AL_PAUSED:
			case AL_STOPPED:
				alSourcePlay(source_music);
				break;
			case AL_PLAYING:
				break;
			}
			//更新声源坐标
			alSource3f(source_music,AL_POSITION,modelx2pos[X],modelx2pos[Y],modelx2pos[Z]);
		}else{//无音
			alSourcePause(source_music);
		}
		
	}
	//释放导弹
	{
		if(sKeyDown){
			if(t-tf_releaseBigcannon>1000/3){//时间间隔大于一定值才能再发射
				tf_releaseBigcannon=t;
				//pdiedbigcannon中取一个
				int ndiedbigcannon=(int)pdiedbigcannonList.size();
				if(ndiedbigcannon!=0){
					Cbigcannon*pbigcannon=pdiedbigcannonList[ndiedbigcannon-1];
					pdiedbigcannonList.pop_back();
					pbigcannon->setbdisapear(false);
					//制作导弹
					float pos[4];//pcell位置
					pcell->getPosInFather(pos);
					matcopy(pcell->RMmat,pbigcannon->RMmat);
					pbigcannon->rotloc100(14);
					pbigcannon->movloc_010(8.0);//让导弹发出点比细菌中心高些，防止刚发出去就爆炸
					pbigcannon->vy=0;
					pbigcannon->visible=true;
					//加入到bigcannonRoot
					bigcannonRoot.addchild(pbigcannon);
					//加入到导弹列表
					pbigcannonList.push_back(pbigcannon);
					//播放声音
					pbigcannon->sourcePos(pos);
					pbigcannon->sourcePlay();	
				}
			}
		}
	}
	//导弹飞行
	{
		int nbigcannon=(int)pbigcannonList.size();
		for(int i=0;i<nbigcannon;i++){
			Cbigcannon&bigcannon=*pbigcannonList[i];
			if(bigcannon.visible){
				float posf[4];
				bigcannon.getPosInFather(posf);
				bigcannon.fly(dt);
				float posn[4];
				bigcannon.getPosInFather(posn);
				bool hitGround=bigcannon.isHitGround(posn,water,ground);
				if(hitGround){//与地面碰撞
					//将bigcannon设置为disapear
					bigcannon.setbdisapear(true);
					//停止声音播放
					bigcannon.sourceStop();
					//放置一个superburst
			
					bigcannon.laySuperBurst(posn,pdiedsuperburstList,psuperburstList,superburstRoot);//放置爆炸
				}else{//不与地面碰撞
					//检测是否与天花板碰撞或地板碰撞
					int whichPartOfHouse=house.smallBallCollisionWithWhichPartOfHouse(posf,posn,bigcannon.Rc);
					//根据collision作出反应
					if(whichPartOfHouse!=POH_NONE){		
						//将bigcannon设置为disapear
						bigcannon.setbdisapear(true);
						//声音停止
						bigcannon.sourceStop();
						//放置一个superburst
						bigcannon.laySuperBurst(posn,pdiedsuperburstList,psuperburstList,superburstRoot);//放置爆炸	
					}	
				}
			}
		}
	}

	//限制大原子爆炸数量
	{
		const int nsuperburst=(int)psuperburstList.size();
		if(nsuperburst>Nsuperburst*0.6){
			//将psuperburstList[0]设置为died
			((Csuperburst*)psuperburstList[0])->setDied(true);
		}
	}
	//大原子爆炸
	{

		int nsuperburst=(int)psuperburstList.size();
		for(int i=0;i<nsuperburst;i++){
			Csuperburst&superburst=*(Csuperburst*)psuperburstList[i];
			superburst.burst(t);
			if(superburst.isdied||superburst.visible==false)continue;
			//设置superburst的各compball颜色
			superburst.updateColorOfcompballs();
		}
		//设置画面整体颜色
		Csuperburst::setSceneColor(psuperburstList,glass);	
//		cout<<glass.visible<<endl;

	}
	//爆炸
	{
		int nburst=(int)pburstList.size();
		for(int i=0;i<nburst;i++){
			pburstList[i]->burst();
		}
	}
	//气泡升起
	{
		if(water.visible){//水可见时才气泡上升
			int nbubble=(int)pbubbleList.size();
			for(int i=0;i<nbubble;i++){
				pbubbleList[i]->rise(dt);
			}
		}
	}
	
	
	//液滴扩散
	{
		int ndrop=(int)pdropList.size();
		for(int i=0;i<ndrop;i++){
			pdropList[i]->diffuse(dt);
		}
	}
	//autoCell着陆
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			float autoCellpos[4];
			autoCell.getPosInFather(autoCellpos);
			float x=autoCellpos[X];
			float z=autoCellpos[Z];
			//着陆
			float groundHorizon=ground.getH(x,z);
		
			if(groundHorizon>water.Horizon){
				autoCell.downToGround_soft(0.5/3,2.0);
			}else{
				autoCell.downToWaterSurface_soft(0.5/3);
			}
		
			
		}
	}

	


	//autoCell转动
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			if(autoCell.visible==false)continue;
			//进行转动
			if(!autoCell.isOnWater()){//在陆上
				autoCell.smartRot(*pcell,dt,t,false);
			}else{//在水上
				autoCell.smartRot(*pcell,dt,t,true);
			}
		}
	}
	//autoCell前进
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			//前进
			if(!autoCell.isOnWater()){//在陆上
				autoCell.advance(*pcell,dt,false);
			}else{//在水上
				autoCell.advance(*pcell,dt,true);
			}
			if(autoCell.visible==false)continue;//如果不可见，只前进即可，不做碰撞检测
			float autoPos[4];
			autoCell.getPosInFather(autoPos);
			//用新位置与house进行碰撞检测
			float e_back[4];//反弹方向
			float d=house.collisionTestWithWall_otherMover(autoPos,autoCell.Rc,e_back);//打入深度
			if(d>0){//与house发生碰撞
				//反弹
				float back[4];//反弹向量
				mul(d,e_back,back);
				autoCell.movloc_inFather(back[X],back[Y],back[Z]);
				//将autoCell的前进方向调为与墙面平行
				//求autoCell的当前朝向向量
				float curdir[4];//当前朝向向量
				//下面四句等价于autoCell.toFather(0,0,-1,0,curdir);
				curdir[0]=-autoCell.RMmat[8];
				curdir[1]=-autoCell.RMmat[9];
				curdir[2]=-autoCell.RMmat[10];
				curdir[3]=-autoCell.RMmat[11];
				//对autoCell调整位姿
				//求与墙面平行的向量，即autoCell的目标朝向向量
				float tgdir[4];//目标朝向向量，tgdir=(back x curdir) x back
				float updn[4];
				cross(back,curdir,updn);
				cross(updn,back,tgdir);
				tgdir[Y]=0;//不能斜
				if(normalize(tgdir)==0){//如果得0向量
					//说明curdir与tgdir共线，不用调整
				}else{//得到tgdir
					//将朝向置为tgdir
					autoCell.setPostureWithWantedDir_inFather(tgdir);
				}
			}
			
		}
	}
	//对每个autoCell计时
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			((CautoCell*)pautoCellList[i])->countTime(dt);
		}
	}
	//每个autoCell蠕动及鞭毛旋转
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			if(((CautoCell*)pautoCellList[i])->visible)((CautoCell*)pautoCellList[i])->wri(t);
			if(((CautoCell*)pautoCellList[i])->visible)((CautoCell*)pautoCellList[i])->updateFlglr(t);
		}
	}
	
	//---------------------景物运动---------------------------------------------
	//protein转动
	{
		int n=(int)pproteinList.size();
		for(int i=0;i<n;i++){
			Cprotein&protein=*pproteinList[i];
			protein.rotloc100(0.25*dt);
			protein.rotloc010(0.375*dt);
		}
	}
	//水波浮动
	if(water.visible)water.wave(t);
	
	//实现water对主角的跟随为了保证水波和贴图的正确，需对二者进行平移
	float water_to_cell[4];
	float waterpos[4];
	water.getPosInFather(waterpos);
	waterpos[Y]=water.Horizon;
	sub(mycellpos,waterpos,water_to_cell);
	//坐标平移向量
	float dx;
	float dz;
	dx=water_to_cell[X];
	dz=water_to_cell[Z];
	water.movRect(dx,dz);
	
	//-------------------主角----------------------------------------------------

	//起跳
	if(aKeyDown){
		if(!pcell->jump){//如果没跳起，才可以跳
			pcell->startJump(0.15);
		}
	}
	//获得house天花板，house地面，地面，水面三个高度
	float houseCeilNorm[4];
	float houseCeilH=house.getH_ceil(mycellpos,pcell->Rc,houseCeilNorm);
	float groundHorizon=ground.getH(mycellpos[X],mycellpos[Z]);
	float houseFloorNorm[4];
	float houseHorizon=house.getH_floor(mycellpos,pcell->Rc,houseFloorNorm);
	float waterHorizon=water.Horizon;
	//pcell中心应悬起的高度
	float H0=3.0;
	//pcell自由落体
	if(pcell->jump){
		pcell->fall(H0,houseCeilH,houseCeilNorm,houseHorizon,groundHorizon,waterHorizon,dt);
	}
	//将pcell的高度调整为max(house地面,地面,水面)
	if(pcell->jump==false){//非跳起时才落于地面
		pcell->adjustHeightToSurface(houseHorizon,houseFloorNorm,groundHorizon,waterHorizon,H0,dt);
	}

	//pcell变色
	pcell->changeCoreColor(t);
	
	//pcell各技能时钟更新
	pcell->timerscount(dt);
	
	//pcell蠕动
	pcell->wri(t);
	//pcell鞭毛旋转
	pcell->updateFlglr(t);
	
	//发射炮弹
	if(dKeyDown){
		if(pcell->fire(t)){//确实发射才播放声音
			alSource3f(source_gun_small, AL_POSITION, mycellpos[X],mycellpos[Y],mycellpos[Z]);
			alSourcePlay(source_gun_small);
		}
		
	}
	
	
	//炮弹运动
	pcell->cannonballsMov(dt);
	//----------------------------------------------*pcell移动
	pcell->storeRMmat();
	pcell->setBmoving(false);
	if(downKeyDown){
		pcell->movBackward(groundHorizon,waterHorizon,H0,dt);
		pcell->setBmoving(true);
	}
	if(upKeyDown){
		pcell->movForward(groundHorizon,waterHorizon,H0,dt);
		pcell->setBmoving(true);
	}
	//播放主角移动时的声音
	{
		//检测source_move_on_ground和source_move_on_water的播放状态
		ALint state_move_on_ground;
		ALint state_move_on_water;
		alGetSourcei(source_move_on_ground, AL_SOURCE_STATE,&state_move_on_ground);
		alGetSourcei(source_move_on_water, AL_SOURCE_STATE,&state_move_on_water);
		int onWhich=pcell->topSurface(houseHorizon,groundHorizon,waterHorizon);
		if(pcell->bmoving==true&&pcell->jump==false){//如果移动且未跳起
			if(onWhich==1||onWhich==2){//如果在地面或house地面
				if(state_move_on_water==AL_PLAYING){//暂停source_move_on_water
					alSourcePause(source_move_on_water);
					//播放咕噜声
					alSource3f(source_water_gulu,AL_POSITION,mycellpos[X],waterHorizon,mycellpos[Z]);
					alSourcePlay(source_water_gulu);
				}
				if(state_move_on_ground!= AL_PLAYING){//如果source_move_on_ground已停止，则再次启动
					alSourcePlay(source_move_on_ground);
				}
				//更新声源位置
				alSource3f(source_move_on_ground,AL_POSITION,mycellpos[X],mycellpos[Y],mycellpos[Z]);
			}else if(onWhich==0){//如果在水面
				if(state_move_on_ground==AL_PLAYING)alSourcePause(source_move_on_ground);//暂停source_move_on_ground
				//检查source_move_on_water的状态
				if(state_move_on_water!= AL_PLAYING){//如果source_move_on_water已停止，则再次启动
					alSourcePlay(source_move_on_water);
				}
				//更新声源位置
				alSource3f(source_move_on_water,AL_POSITION,mycellpos[X],mycellpos[Y],mycellpos[Z]);
			}
		}else{//如果跳起
			if(state_move_on_ground==AL_PLAYING)alSourcePause(source_move_on_ground);//暂停source_move_on_ground
			if(state_move_on_water==AL_PLAYING){//暂停source_move_on_water
				alSourcePause(source_move_on_water);
				//播放咕噜声
				alSource3f(source_water_gulu,AL_POSITION,mycellpos[X],waterHorizon,mycellpos[Z]);
				alSourcePlay(source_water_gulu);
			}
		}
	}
	//------------------------------*pcell移动后执行其与静态物体的碰撞检测，如果碰撞，则恢复
	//取得pcell移动后的新坐标
	pcell->getPosInFather(mycellpos);
	//pcell与house的碰撞
	float e_back[4];
	float up[4];//世界空间中的向上向量
	veccopy(pcell->RMmat+4,up);//等价于pcell->toFather(0,1,0,0,up)，得到up
	float d=house.collisionTestWithWall_multiPoint(mycellpos,pcell->Rc,up,e_back);
	if(d>0){
		//恢复
		matcopy(pcell->RMmatStore,pcell->RMmat);
		float back[4];
		mul(d*0.15,e_back,back);
		back[Y]=0;
		pcell->movloc_inFather(back[X],back[Y],back[Z]);
	}
	
	
	//----------------------------------------------------*pcell旋转
	if(leftKeyDown){
		if(groundHorizon<water.Horizon&&mycellpos[Y]-H0<=water.Horizon){
			pcell->rotloc_pushWater_010(0.175*dt);
		}else{
			pcell->rotloc010(0.175*dt);
		}
		pcell->bmoving=true;
	}
	if(rightKeyDown){
		if(groundHorizon<water.Horizon&&mycellpos[Y]-H0<=water.Horizon){
			pcell->rotloc_pushWater_010(-0.175*dt);
		}else{
			pcell->rotloc010(-0.175*dt);
		}
		pcell->bmoving=true;
	}
	//-------------将出界的物体移回界内
	movInBond(width_mov,height_mov,dt,t);
	//-------------删除死亡物体
	ridDiedThings();
	//-------------生成新物体
	rebrithThings(t);
	//-----------------------------更新LOD等级或点线宽度
	updateLODlevelAndPointOrLineSize();
	//-------------------------------------------------------
	if(cheating){
		pcell->Ngun=4;
		init4(pcell->eneBallColorList[0],0,1,0,1);
		init4(pcell->eneBallColorList[1],0,1,0,1);
		init4(pcell->eneBallColorList[2],0,1,0,1);
		init4(pcell->eneBallColorList[3],0,1,0,1);
		pcell->setjumpable(true);
		pcell->speedup();
	}

}
