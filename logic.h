
inline void generateCollsionGrids(){
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
	//----------------------------������ײ����
	//����autoCell��ײ����
	autoCellCgrid.clearContent();
	autoCellCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	autoCellCgrid.fillContent(pautoCellList);
	//����amac��ײ����
	amacCgrid.clearContent();
	amacCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	amacCgrid.fillContent(pamacList);
	//����ene��ײ����
	eneCgrid.clearContent();
	eneCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	eneCgrid.fillContent(peneList);
	//����gene��ײ����
	geneCgrid.clearContent();
	geneCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	geneCgrid.fillContent(pgeneList);
	//����tree��ײ����
	treeCgrid.clearContent();
	treeCgrid.setCenter(mycellpos[X],mycellpos[Z]);
	treeCgrid.fillContent(ptreeList);
}
inline void rebrithThings(long t){
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
	if(rand()%5==0){
		//��������
		//�����������
		int x=mycellpos[X]+(rand()%(int)areaWidth_god-areaWidth_god/2)*0.5;
		int z=mycellpos[Z]+(rand()%(int)areaHeight_god-areaHeight_god/2)*0.5;
		//��������θ��ǣ�������bubble������
		float groundHorizon=ground.getH(x,z);
		if(groundHorizon<water.Horizon){//��ˮ�棬��������
			Cbubble*pbubble=NULL;
			//��pdiedbubbleList���Ƿ���Ԫ��
			if((int)pdiedbubbleList.size()!=0){
				//��pdiedbubbleList�����һ��Ԫ��ȡ����
				pbubble=pdiedbubbleList[(int)pdiedbubbleList.size()-1];
				pdiedbubbleList.pop_back();
				pbubble->reSet();
				pbubbleList.push_back(pbubble);
				pbubble->movloc(x,groundHorizon,z);
				bubbleRoot.addchild(pbubble);
				//�������
				if(rand()%2==0){//����������ô�ܼ�
					pbubble->sourcePos(x,groundHorizon,z);
					pbubble->sourcePlay();
				}
			}
			
		}
	}
	if(rand()%7==0){
		//����Һ��

		//��������
		int x=mycellpos[X]+(rand()%(int)(areaWidth_god)-areaWidth_god/2)*0.6;
		int z=mycellpos[Z]+(rand()%(int)(areaHeight_god)-areaHeight_god/2)*0.6;
		//�ж����ڵ��滹����ˮ��
		float groundHorizon=ground.getH(x,z);
		if(groundHorizon<water.Horizon){//��ˮ�棬����Һ��
			Cdrop*pdrop=NULL;
			//��pdieddropList���Ƿ���Ԫ��
			if((int)pdieddropList.size()!=0){
				//��pdieddropList�����һ��Ԫ��ȡ����
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
				//���뵽dropRoot
				dropRoot.addchild(&drop);
			}

		}

	
	}
	//������һЩ��������Ķ��������ڷ�Ƶ�����
	if(rand()%10==0){

		//����autoCell
		//���pdiedautoCellList����Ԫ�أ�������ó�һ����������
		int ndiedautoCell=(int)pdiedautoCellList.size();
		if(ndiedautoCell!=0){//��Ԫ�أ�ѭ������
			//��pdiedautoCellList���ó�һ��Ԫ��
			CautoCell*pautoCell=(CautoCell*)pdiedautoCellList[ndiedautoCell-1];
			pdiedautoCellList.pop_back();
			//��pautoCell���³�ʼ��
			const float vlen=1.5/3/40;//������ϸ���ٶ��ѷ����˱仯�������ٶ������³�ʼ��
			int A=rand()*piDiv180;
			float vx=vlen*fastcos(A);
			float vz=vlen*fastsin(A);
			pautoCell->setV(vx,0,vz);
			int x=mycellpos[X]+rand()%(int)(width_mov)-(width_mov)/2;
			int z=mycellpos[Z]+rand()%(int)(height_mov)-(height_mov)/2;
			pautoCell->movloc_101(x,z);
			pautoCell->bflglr=false;//�ޱ�ë
			pautoCell->bshell=false;//�޿�
			pautoCell->foot=false;//�޽ţ��нŵ��Ǹ��㣬ֻ��û����ɫ��
			pautoCell->knee=false;//��ϥ����ϥ���Ǹ��㣬ֻ��û����ɫ��
			pautoCell->timecount=0;
			pautoCell->life=2;
			cellRoot.addchild(pautoCell);
			//��½(������ڼ��뵽cellRoot֮��)
			float groundHorizon=ground.getH(x,z);
			if(groundHorizon>water.Horizon){
				pautoCell->downToGround(2.0);
			}else{
				pautoCell->downToWaterSurface();

			}
			//��pautoCell���뵽pautoCellList
			pautoCellList.push_back(pautoCell);
		}

	}
	if(rand()%10==0){
		//����amac
		//��pdiedamacList����һ��
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
			//��½
			float groundHorizon=ground.getH(x,z);
			if(groundHorizon>water.Horizon){
				amac.downToGround(2.0);
			}else{
				amac.downToWaterSurface();
				
			}
		}

	}
	if(rand()%10==0){
		//����gene
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
	
			//��½
			float groundHorizon=ground.getH(x,z);
			if(groundHorizon>water.Horizon){
				gene.downToGround(2.0);
			}else{
				gene.downToWaterSurface();
				
			}
			
		}
	}
	if(rand()%10==0){
		//����ene
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
			//��½
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
	//�������modelx�Ƶ�����
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
	//�������tree�Ƶ�����
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
				//��½
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

	
	//�������amac�Ƶ�����
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
				//��½
				float groundHorizon=ground.getH(amacnewpos[X],amacnewpos[Z]);
				if(groundHorizon>water.Horizon){
					amac.downToGround(2.0);
				}else{
					amac.downToWaterSurface();
					
				}
			}
			
		}
	}
	//amac�ƶ�
	{
		int namac=(int)pamacList.size();
		for(int i=0;i<namac;i++){
			Camac&amac=*(Camac*)pamacList[i];
			if(amac.visible==false)continue;
			if(amac.isOnWater()){
				amac.floatOnWater(dt);
				amac.rotloc010(0.6*dt);
				//�ж���house����ײ
				float amacpos[4];
				amac.getPosInFather(amacpos);
				float e_back[4];
				float d=house.collisionTestWithWall_singlePoint(amacpos,amac.Rc,e_back);
				if(d>0){//��ǽ�ڷ�����ײ
					//�ƶ�
					e_back[Y]=0;//���Y��������ֹ���ӷ����Ǳˮ��
					float back[4];
					mul(d,e_back,back);
					amac.movloc_inFather(back[X],back[Y],back[Z]);
					//�ı��ٶ�
					float *v=amac.v;//�����ٶ�
					//�����ٶ�v2=v-2*dot(v,e_back)*e_back
					//��������k=-2*dot(v,e_back)��u=k*e_back����v2=v+u��
					float v2[4];
					float k=-2*dot(v,e_back);
					float u[4];
					mul(k,e_back,u);
					add(v,u,v2);//�õ�v2
					//��v2��дv
					veccopy(v2,v);
				}
			}	
			
		}
	}
	
	//�������ene�Ƶ�����
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
				//��½
				float groundHorizon=ground.getH(enenewpos[X],enenewpos[Z]);
				if(groundHorizon>water.Horizon){
					ene.downToGround(2.0);
				}else{
					ene.downToWaterSurface();		
				}
			}
			
		}
	}
	//ene�ƶ�
	{
		int nene=(int)peneList.size();
		for(int i=0;i<nene;i++){
			Cene&ene=*(Cene*)peneList[i];
			if(ene.visible==false)continue;
			if(ene.isOnWater()){
				ene.floatOnWater(dt);
				ene.rotloc010(0.9*dt);
				//�ж���house����ײ
				float enepos[4];
				ene.getPosInFather(enepos);
				float e_back[4];
				float d=house.collisionTestWithWall_singlePoint(enepos,ene.Rc,e_back);
				if(d>0){//��ǽ�ڷ�����ײ
					//�ƶ�
					e_back[Y]=0;//���Y��������ֹ���ӷ����Ǳˮ��
					float back[4];
					mul(d,e_back,back);
					ene.movloc_inFather(back[X],back[Y],back[Z]);
					//�ı��ٶ�
					float *v=ene.v;//�����ٶ�
					//�����ٶ�v2=v-2*dot(v,e_back)*e_back
					//��������k=-2*dot(v,e_back)��u=k*e_back����v2=v+u��
					float v2[4];
					float k=-2*dot(v,e_back);
					float u[4];
					mul(k,e_back,u);
					add(v,u,v2);//�õ�v2
					//��v2��дv
					veccopy(v2,v);
				}
			}
		}
	}
	
	//�������gene�Ƶ�����
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
				//��½
				float groundHorizon=ground.getH(genenewpos[X],genenewpos[Z]);
				if(groundHorizon>water.Horizon){
					gene.downToGround(2.0);
				}else{
					gene.downToWaterSurface();
					
				}
			}
			
		}
	}
	//gene�ƶ�
	{
		int ngene=(int)pgeneList.size();
		for(int i=0;i<ngene;i++){
			Cgene&gene=*(Cgene*)pgeneList[i];
			if(gene.visible==false)continue;
			if(gene.isOnWater()){
				gene.floatOnWater(dt);
				//�ж���house����ײ
				float genepos[4];
				gene.getPosInFather(genepos);
				float e_back[4];
				float d=house.collisionTestWithWall_singlePoint(genepos,gene.Rc,e_back);
				if(d>0){//��ǽ�ڷ�����ײ
					//�ƶ�
					e_back[Y]=0;//���Y��������ֹ���ӷ����Ǳˮ��
					float back[4];
					mul(d,e_back,back);
					gene.movloc_inFather(back[X],back[Y],back[Z]);
					//�ı��ٶ�
					float *v=gene.v;//�����ٶ�
					//�����ٶ�v2=v-2*dot(v,e_back)*e_back
					//��������k=-2*dot(v,e_back)��u=k*e_back����v2=v+u��
					float v2[4];
					float k=-2*dot(v,e_back);
					float u[4];
					mul(k,e_back,u);
					add(v,u,v2);//�õ�v2
					//��v2��дv
					veccopy(v2,v);
				}
			}
		}
	}
	
	//�������autoCell�Ƶ�����
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
		
	//-----------�����б���ѭ���г��ȷ����仯�����������Ͻ���ǰ�̻�����ѭ������Ӧ��n--
	//��life��Ϊ0��autoCellɾ��
	{
		int nautoCell=(int)pautoCellList.size();
		int count=0;
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			if(autoCell.life<=0){
				//���ǻ�÷���
				pcell->score+=10;
				//�ӳ�������ժ����
				cellRoot.ridchild(&autoCell);
				//���뵽�����о��б�
				pdiedautoCellList.push_back(&autoCell);
				count++;
			}else{
				if(count!=0)pautoCellList[i-count]=&autoCell;
			}
		}
		pautoCellList.resize(nautoCell-count);
	}

	//�����Ϊdisapear���ڵ�ɾ��
	{
		int ncannonball=(int)pcell->pcannonballList.size();
		int count=0;
		for(int i=0;i<ncannonball;i++){
			Ccannonball&cannonball=*(Ccannonball*)pcell->pcannonballList[i];
			if(cannonball.isdisapear()){
				CweaponMover::cannonballRoot.ridchild(&cannonball);//���ڵ���cannonballRoot��ȡ��
				//���뵽pcell->pdiedcannonballList
				pcell->pdiedcannonballList.push_back(&cannonball);
				count++;
			}else{
				if(count!=0)pcell->pcannonballList[i-count]=&cannonball;
			}
		}
		pcell->pcannonballList.resize(ncannonball-count);
	}
	//�����Ϊdisapear��amacɾ��
	{
		int namac=(int)pamacList.size();
		int count=0;
		for(int i=0;i<namac;i++){
			Camac&amac=*(Camac*)pamacList[i];
			if(amac.isdisapear()){
				//��molecuRoot��ժ����
				molecuRoot.ridchild(&amac);
				//�ӵ�pdiedamacList��
				pdiedamacList.push_back(&amac);
				count++;
			}else{
				if(count!=0)pamacList[i-count]=&amac;
				
			}
		}
		pamacList.resize(namac-count);
	}
	//�����Ϊdisapear��eneɾ��
	{
		int nene=(int)peneList.size();
		int count=0;
		for(int i=0;i<nene;i++){
			Cene&ene=*(Cene*)peneList[i];
			if(ene.isdisapear()){
				//��molecuRoot��ժ����
				molecuRoot.ridchild(&ene);
				//���뵽pdiedeneList
				pdiedeneList.push_back(&ene);
				count++;
			}else{
				if(count!=0)peneList[i-count]=&ene;
			}
		}
		peneList.resize(nene-count);
	}
	//�����Ϊdisapear��geneɾ��
	{
		int ngene=(int)pgeneList.size();
		int count=0;
		for(int i=0;i<ngene;i++){
			Cgene&gene=*(Cgene*)pgeneList[i];
			if(gene.isdisapear()){
				//��molecuRoot��ժ����
				molecuRoot.ridchild(&gene);
				//���뵽pdiedgeneList
				pdiedgeneList.push_back(&gene);
				count++;
			}else{
				if(count!=0)pgeneList[i-count]=&gene;
			}
		}
		pgeneList.resize(ngene-count);
	}
	//�����Ϊdisapear��proteinɾ��
	{
		int nprotein=(int)pproteinList.size();
		int count=0;
		for(int i=0;i<nprotein;i++){
			Cprotein&protein=*(Cprotein*)pproteinList[i];
			if(protein.isdisapear()){
				//��proteinRoot��ժ����
				proteinRoot.ridchild(&protein);
				//���뵽pdiedproteinList
				pdiedproteinList.push_back(&protein);
				count++;
			}else{
				if(count!=0)pproteinList[i-count]=&protein;
			}
		}
		pproteinList.resize(nprotein-count);
	}
	
	//����ѽ���������
	{
		int nbubble=(int)pbubbleList.size();
		int count=0;
		for(int i=0;i<nbubble;i++){
			Cbubble&bubble=*pbubbleList[i];
			if(bubble.isdied){
				//�ӳ�������ժ����
				bubbleRoot.ridchild(&bubble);
				//���뵽pdiedbubbleList
				pdiedbubbleList.push_back(&bubble);
				count++;
			}else{
				if(count!=0)pbubbleList[i-count]=&bubble;
			}
		}
		pbubbleList.resize(nbubble-count);
	}
	//����ѽ�����Һ��
	{
		int ndrop=(int)pdropList.size();
		int count=0;
		for(int i=0;i<ndrop;i++){
			Cdrop&drop=*pdropList[i];
			if(drop.isdied){
				//��dropRoot��ժ����
				dropRoot.ridchild(&drop);
				//���뵽pdieddropList
				pdieddropList.push_back(&drop);
				count++;
			}else{
				if(count!=0)pdropList[i-count]=&drop;
			}
		}
		pdropList.resize(ndrop-count);
	}
	
	//��disapear��bigcannon����pdiedbigcannonList�б�
	{
		int nbigcannon=(int)pbigcannonList.size();
		int count=0;
		for(int i=0;i<nbigcannon;i++){
			Cbigcannon&bigcannon=*pbigcannonList[i];
			if(bigcannon.getbdisapear()){
				//��bigcannonRoot��ժ����
				bigcannonRoot.ridchild(&bigcannon);
				//���뵽pdiedbigcannonList��
				pdiedbigcannonList.push_back(&bigcannon);
				count++;
			}else{
				if(count!=0)pbigcannonList[i-count]=&bigcannon;
			}
		}
		pbigcannonList.resize(nbigcannon-count);
	}
	
	//���ѽ�����superburstɾ��
	{
		int nsuperburst=(int)psuperburstList.size();
		int count=0;
		for(int i=0;i<nsuperburst;i++){
			Csuperburst&superburst=*(Csuperburst*)psuperburstList[i];
			if(superburst.isdied){
				//��superburstRoot��ժ����
				superburstRoot.ridchild(&superburst);
				//���뵽pdiedsuperburstList
				pdiedsuperburstList.push_back(&superburst);
				count++;
			}else{
				if(count!=0)psuperburstList[i-count]=&superburst;
			}
		}
		psuperburstList.resize(nsuperburst-count);
	}
	//���ѽ�����burst����pdiedburstList�б�
	{
		int nburst=(int)pburstList.size();
		int count=0;
		for(int i=0;i<nburst;i++){
			Cburst&burst=*pburstList[i];
			if(burst.isdied){
				//�ӳ�������ժ����
				burstRoot.ridchild(&burst);
				//���뵽pdiedburstList��
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
	//------------------------------��ײ���(��ײ������ǰ�棬��ֹ��pListʧЧ)------------------------------------------
	//autoCell֮�����ײ��ʹ���������
	{
		CcollisionTestProperty ctp;ctp.set(true,true,true);
		CcollisionReactProperty crp;crp.set(true,false,1);
		collision_modelList_modelList(pautoCellList,pautoCellList,autoCellCgrid,ctp,crp,
			collisionReact_BothRollAndBack);
	}	
	
	//autoCell��tree��ײ��ʹ���������
	{
		CcollisionTestProperty ctp;ctp.set(true,true,true);
		CcollisionReactProperty crp;crp.set(true,false,1);
		collision_modelList_modelList(pautoCellList,ptreeList,treeCgrid,ctp,crp,
			collisionReact_Back_Still);
	}
	
	//cannonball��autoCell��ײ��ʹ���������
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_modelList_modelList(pcell->pcannonballList,pautoCellList,autoCellCgrid,ctp,crp,
			collisionReact_cannonball_autoCell);
	
	}
	//superburst��autocell��ײ��ʹ���������
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_modelList_modelList(psuperburstList,pautoCellList,autoCellCgrid,ctp,crp,
			collisionReact_superburst_autoCell);

	}
	//cannonball�������ײ
	{
		int ncannon=(int)pcell->pcannonballList.size();
		for(int i=0;i<ncannon;i++){
			Ccannonball&cannonball=*(Ccannonball*)pcell->pcannonballList[i];
			float ballpos[4];
			cannonball.getPosInFather(ballpos);
			float groundH=ground.getH(ballpos[X],ballpos[Z]);
			if(ballpos[Y]<=groundH){
				//cannonball��ʧ
				cannonball.disapear();
				//���ڵ���ʧ������һ����ը
				layBurst(ballpos[X],groundH,ballpos[Z]);
			}
		}
	}
	//*pcell��autoCell��ײ��ʹ���������
	{
		CcollisionTestProperty ctp;ctp.set(true,true,true);
		CcollisionReactProperty crp;crp.set(true,false,1);
		collision_model_modelList(pcell,pautoCellList,autoCellCgrid,ctp,crp,
			collisionReact_pcell_autoCell);
	}
	//*pcell��tree��ײ��ʹ���������
	{
		CcollisionTestProperty ctp;ctp.set(true,true,false);
		CcollisionReactProperty crp;crp.set(true,true,0.25);
		collision_model_modelList(pcell,ptreeList,treeCgrid,ctp,crp,
			collisionReact_Back_Still);
	}
	//*pcell��amac��ײ��ʹ���������
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_model_modelList(pcell,pamacList,amacCgrid,ctp,crp,
			collisionReact_pcell_amac);
	}
	//*pcell��ene��ײ
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_model_modelList(pcell,peneList,eneCgrid,ctp,crp,
			collisionReact_pcell_ene);
	}
	//*pcell��gene��ײ
	{
		CcollisionTestProperty ctp;ctp.set(false,false,false);
		CcollisionReactProperty crp;crp.set(false,false,1);
		collision_model_modelList(pcell,pgeneList,geneCgrid,ctp,crp,
			collisionReact_pcell_gene);

	}
	

	//cannonball��house��ײ
	{
		int ncannonball=(int)pcell->pcannonballList.size();
		for(int i=0;i<ncannonball;i++){
			Ccannonball&cannonball=*(Ccannonball*)pcell->pcannonballList[i];
			//��ǰ�ڵ�λ��
			float ballpos[4];
			cannonball.getPosInFather(ballpos);
			//ǰһʱ���ڵ�λ��
			float *ballposf=cannonball.posf;
			//�ж��߶�ballposf--ballpos�Ƿ���house�ཻ
			float p[4];//��������
			if(house.collisionTest_lineSeg(ballposf,ballpos,p)){
				//cannonball��ʧ
				cannonball.disapear();
				//���ڵ���ʧ������һ����ը
				Cburst*pburst=layBurst(p[X],p[Y],p[Z]);
				if(pburst!=NULL){
					//pburst��Ч������ǽ���ϵ�����
					pburst->sourcePos(p);
					pburst->sourcePlay();
				}
			}
		}
	}
	//modelx��pcell��ײ
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
			if(collision){//��ײ
				collisionReact_Back_Back(cp,crp);
				//����
				{
					bool allstopped;//�Ƿ�����������ֹͣ��
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
						//����modelx��pcell�ĳ�����������ĸ�����
						float dir_modelx[4];
						modelx.getDir(dir_modelx);
						if(dot(cp.n1,dir_modelx)>=0){//ӭ������
							//����hellow, i'm beavis
							alSource3f(source_modelx_hellow_i_m_beavis,AL_POSITION,pos2[X],pos2[Y],pos2[Z]);
							alSourcePlay(source_modelx_hellow_i_m_beavis);
						}else{//׷β
							//����what are you doing
							alSource3f(source_modelx_what_are_you_doing,AL_POSITION,pos2[X],pos2[Y],pos2[Z]);
							alSourcePlay(source_modelx_what_are_you_doing);
						}	
					}
				}
				
			}
		}	
	}
	//modelx��modelx2��ײ
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
			if(collision){//��ײ
				collisionReact_Back_Still(cp,crp);
			}
		}	
	}
	//modelx2��pcell��ײ
	{
		if(modelx2.visible){
			float *pos1=mycellpos;
			float pos2[4];
			modelx2.getPosInFather(pos2);
			float R1=pcell->Rc;
			float R2=modelx2.Rc;
			float dis2=pow2(pos1[X]-pos2[X])+pow2(pos1[Y]-pos2[Y])+pow2(pos1[Z]-pos2[Z]);
			if(dis2<=pow2(R1+R2)){//��ײ
				
				float d=fabs((R1+R2)-CarmSqrt(dis2));//��ײ�ķ������
				                                     //����fabs��Ϊ�˱���dΪ��ֵ��֮���Ի����dΪ��ֵ�����������ΪCarmSqrt����ȷ
				//pcell����һ��
				//���������
				float n1[4];//pcell�ķ�������
				sub((float(&)[4])(*pos1),(float(&)[4])(*pos2),n1);
				n1[Y]=0;
				float back[4];//pcell�Ļ�������
				if(normalize(n1)!=0){
					mul(d,n1,back);//�õ�back
					//pcell����
					pcell->movloc_inFather(back[X],back[Y],back[Z]);
				}
			}
		}	
	}
	//modelx��autoCell��ײ��ʹ���������
	{
		if(modelx.visible){
			CcollisionTestProperty ctp;ctp.set(true,true,true);
			CcollisionReactProperty crp;crp.set(true,false,1);
			collision_model_modelList(&modelx,pautoCellList,autoCellCgrid,ctp,crp,
				collisionReact_Back_RollAndBack);
		}
	}
	//modelx2��autoCell��ײ��ʹ���������
	{
		if(modelx2.visible){
			CcollisionTestProperty ctp;ctp.set(true,true,true);
			CcollisionReactProperty crp;crp.set(true,false,1);
			collision_model_modelList(&modelx2,pautoCellList,autoCellCgrid,ctp,crp,
				collisionReact_Still_RollAndBack);
		}
	}
	
	
	//metalBall��pcell��ײ
	{
		if(metalBall.visible){
			float *pos1=mycellpos;
			float pos2[4];
			metalBall.getPosInFather(pos2);
			float R1=pcell->Rc;
			float R2=metalBall.Rc;
			float dis2=pow2(pos1[X]-pos2[X])+pow2(pos1[Y]-pos2[Y])+pow2(pos1[Z]-pos2[Z]);
			if(dis2<=pow2(R1+R2)){//��ײ
				
				float d=fabs((R1+R2)-CarmSqrt(dis2));//��ײ�ķ������
				                                     //����fabs��Ϊ�˱���dΪ��ֵ��֮���Ի����dΪ��ֵ�����������ΪCarmSqrt����ȷ
				//pcell����һ��
				//���������
				float n1[4];//pcell�ķ�������
				sub((float(&)[4])(*pos1),pos2,n1);
				n1[Y]=0;
				float back[4];//pcell�Ļ�������
				if(normalize(n1)!=0){
					mul(d,n1,back);//�õ�back
					//pcell����
					pcell->movloc_inFather(back[X],back[Y],back[Z]);
				}
			}
		}	
	}
	
	
	//pcell��protein����ײ���
	{
		int n=(int)pproteinList.size();
		for(int i=0;i<n;i++){
			Cprotein&protein=*pproteinList[i];
			if(protein.visible){
				float R1=pcell->Rc;
				float R2=protein.Rc;
				float *pos1=mycellpos;
				float pos2[4];//protein������
				protein.getPosInFather(pos2);
				if(pow2(pos1[X]-pos2[X])+pow2(pos1[Y]-pos2[Y])+pow2(pos1[Z]-pos2[Z])<pow2(R1+R2)
					&&fabs(pos1[Y]-pos2[Y])<R2+R1/6){//��ײ
						//protein��ʧ
						protein.disapear();
						NproteinEaten++;
						//����Ƿ����е�protein���ѳ���
						if(NproteinEaten==NproteinMax){
							missionDone=true;
						}
						//����mission_done��Ч
						alSource3f(source_eatProtein,AL_POSITION,pos1[X],pos1[Y],pos1[Z]);//��pos1������pos2
						alSourcePlay(source_eatProtein);
						if(missionDone){//���missionDone����Ҫͬʱ����ף������
							alSource3f(source_mission_done,AL_POSITION,pos1[X],pos1[Y],pos1[Z]);//��pos1������pos2
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
	////////////////////  �Ը��б�������  ///////////////////////
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
	//��ײ���
	{
		//������ײ����
		generateCollsionGrids();
		//��ײ����߼�
		logic_collsions(dt,t);
	}
	//�����ڵ��Ƿ񼺴ﵽ���
	{
		int ncanonball=(int)pcell->pcannonballList.size();
		for(int i=0;i<ncanonball;i++){
			Ccannonball&cannonball=*(Ccannonball*)pcell->pcannonballList[i];
			//�������pcell��Զ������ʧ
			float cannonballpos[4];
			cannonball.getPosInFather(cannonballpos);
			float vec[4];
			sub(mycellpos,(float(&)[4])(*cannonballpos),vec);
			if(getLen_2(vec)>=200.0*200.0){
				cannonball.disapear();
			}
		}
	}
	//--------------------�ƶ�----------------------------------
	//modelx�˶�
	{
		//�����ܿɲ��ɼ�����Ҫ�ƶ��������䵽������
		if(modelx.visible){
			modelx.autoMov(house,dt,t);
		}else{
			modelx.autoMov_whenVisibleIsFalse(dt,t);
		}
		modelx.downToGround_soft();
		//���ܿɼ���񣬶��ǽ��ж����������������н�����Ҫ�κο�����ֻ��ʱ��++����
		modelx.modelfbx.animationAdvance_rollback(1*dt);
		
	//	cout<<"modelx.visible:"<<modelx.visible<<endl;
	//	cout<<"modelx.visible_reflect:"<<modelx.visible_reflect<<endl;

	}
	//modelx2
	{
	//	modelx2.downToGround_soft();
		//���ܿɼ���񶼲����������������������κο���������ʱ��++����
		modelx2.modelfbx.animationAdvance_rollback(1*dt);

		//��pcell��modelx2�ľ���ƽ��
		float modelx2pos[4];
		modelx2.getPosInFather(modelx2pos);
		float d2=pow2(mycellpos[X]-modelx2pos[X])+pow2(mycellpos[Y]-modelx2pos[Y])+pow2(mycellpos[Z]-modelx2pos[Z]);
		const float peakValue=6;
		const float maxDis=200.0;
		const float k=-peakValue/maxDis;
		float d=fabs(CarmSqrt(d2));
		float currentValue=max(0,peakValue+k*d);
		alSourcef(source_music, AL_GAIN,currentValue);
		if(currentValue>0){//����
			ALint state;
			alGetSourcei(source_music, AL_SOURCE_STATE,&state);
			switch(state){//AL_SOURCE_STATEֻ��������
			case AL_INITIAL:
			case AL_PAUSED:
			case AL_STOPPED:
				alSourcePlay(source_music);
				break;
			case AL_PLAYING:
				break;
			}
			//������Դ����
			alSource3f(source_music,AL_POSITION,modelx2pos[X],modelx2pos[Y],modelx2pos[Z]);
		}else{//����
			alSourcePause(source_music);
		}
		
	}
	//�ͷŵ���
	{
		if(sKeyDown){
			if(t-tf_releaseBigcannon>1000/3){//ʱ��������һ��ֵ�����ٷ���
				tf_releaseBigcannon=t;
				//pdiedbigcannon��ȡһ��
				int ndiedbigcannon=(int)pdiedbigcannonList.size();
				if(ndiedbigcannon!=0){
					Cbigcannon*pbigcannon=pdiedbigcannonList[ndiedbigcannon-1];
					pdiedbigcannonList.pop_back();
					pbigcannon->setbdisapear(false);
					//��������
					float pos[4];//pcellλ��
					pcell->getPosInFather(pos);
					matcopy(pcell->RMmat,pbigcannon->RMmat);
					pbigcannon->rotloc100(14);
					pbigcannon->movloc_010(8.0);//�õ����������ϸ�����ĸ�Щ����ֹ�շ���ȥ�ͱ�ը
					pbigcannon->vy=0;
					pbigcannon->visible=true;
					//���뵽bigcannonRoot
					bigcannonRoot.addchild(pbigcannon);
					//���뵽�����б�
					pbigcannonList.push_back(pbigcannon);
					//��������
					pbigcannon->sourcePos(pos);
					pbigcannon->sourcePlay();	
				}
			}
		}
	}
	//��������
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
				if(hitGround){//�������ײ
					//��bigcannon����Ϊdisapear
					bigcannon.setbdisapear(true);
					//ֹͣ��������
					bigcannon.sourceStop();
					//����һ��superburst
			
					bigcannon.laySuperBurst(posn,pdiedsuperburstList,psuperburstList,superburstRoot);//���ñ�ը
				}else{//���������ײ
					//����Ƿ����컨����ײ��ذ���ײ
					int whichPartOfHouse=house.smallBallCollisionWithWhichPartOfHouse(posf,posn,bigcannon.Rc);
					//����collision������Ӧ
					if(whichPartOfHouse!=POH_NONE){		
						//��bigcannon����Ϊdisapear
						bigcannon.setbdisapear(true);
						//����ֹͣ
						bigcannon.sourceStop();
						//����һ��superburst
						bigcannon.laySuperBurst(posn,pdiedsuperburstList,psuperburstList,superburstRoot);//���ñ�ը	
					}	
				}
			}
		}
	}

	//���ƴ�ԭ�ӱ�ը����
	{
		const int nsuperburst=(int)psuperburstList.size();
		if(nsuperburst>Nsuperburst*0.6){
			//��psuperburstList[0]����Ϊdied
			((Csuperburst*)psuperburstList[0])->setDied(true);
		}
	}
	//��ԭ�ӱ�ը
	{

		int nsuperburst=(int)psuperburstList.size();
		for(int i=0;i<nsuperburst;i++){
			Csuperburst&superburst=*(Csuperburst*)psuperburstList[i];
			superburst.burst(t);
			if(superburst.isdied||superburst.visible==false)continue;
			//����superburst�ĸ�compball��ɫ
			superburst.updateColorOfcompballs();
		}
		//���û���������ɫ
		Csuperburst::setSceneColor(psuperburstList,glass);	
//		cout<<glass.visible<<endl;

	}
	//��ը
	{
		int nburst=(int)pburstList.size();
		for(int i=0;i<nburst;i++){
			pburstList[i]->burst();
		}
	}
	//��������
	{
		if(water.visible){//ˮ�ɼ�ʱ����������
			int nbubble=(int)pbubbleList.size();
			for(int i=0;i<nbubble;i++){
				pbubbleList[i]->rise(dt);
			}
		}
	}
	
	
	//Һ����ɢ
	{
		int ndrop=(int)pdropList.size();
		for(int i=0;i<ndrop;i++){
			pdropList[i]->diffuse(dt);
		}
	}
	//autoCell��½
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			float autoCellpos[4];
			autoCell.getPosInFather(autoCellpos);
			float x=autoCellpos[X];
			float z=autoCellpos[Z];
			//��½
			float groundHorizon=ground.getH(x,z);
		
			if(groundHorizon>water.Horizon){
				autoCell.downToGround_soft(0.5/3,2.0);
			}else{
				autoCell.downToWaterSurface_soft(0.5/3);
			}
		
			
		}
	}

	


	//autoCellת��
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			if(autoCell.visible==false)continue;
			//����ת��
			if(!autoCell.isOnWater()){//��½��
				autoCell.smartRot(*pcell,dt,t,false);
			}else{//��ˮ��
				autoCell.smartRot(*pcell,dt,t,true);
			}
		}
	}
	//autoCellǰ��
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			CautoCell&autoCell=*(CautoCell*)pautoCellList[i];
			//ǰ��
			if(!autoCell.isOnWater()){//��½��
				autoCell.advance(*pcell,dt,false);
			}else{//��ˮ��
				autoCell.advance(*pcell,dt,true);
			}
			if(autoCell.visible==false)continue;//������ɼ���ֻǰ�����ɣ�������ײ���
			float autoPos[4];
			autoCell.getPosInFather(autoPos);
			//����λ����house������ײ���
			float e_back[4];//��������
			float d=house.collisionTestWithWall_otherMover(autoPos,autoCell.Rc,e_back);//�������
			if(d>0){//��house������ײ
				//����
				float back[4];//��������
				mul(d,e_back,back);
				autoCell.movloc_inFather(back[X],back[Y],back[Z]);
				//��autoCell��ǰ�������Ϊ��ǽ��ƽ��
				//��autoCell�ĵ�ǰ��������
				float curdir[4];//��ǰ��������
				//�����ľ�ȼ���autoCell.toFather(0,0,-1,0,curdir);
				curdir[0]=-autoCell.RMmat[8];
				curdir[1]=-autoCell.RMmat[9];
				curdir[2]=-autoCell.RMmat[10];
				curdir[3]=-autoCell.RMmat[11];
				//��autoCell����λ��
				//����ǽ��ƽ�е���������autoCell��Ŀ�곯������
				float tgdir[4];//Ŀ�곯��������tgdir=(back x curdir) x back
				float updn[4];
				cross(back,curdir,updn);
				cross(updn,back,tgdir);
				tgdir[Y]=0;//����б
				if(normalize(tgdir)==0){//�����0����
					//˵��curdir��tgdir���ߣ����õ���
				}else{//�õ�tgdir
					//��������Ϊtgdir
					autoCell.setPostureWithWantedDir_inFather(tgdir);
				}
			}
			
		}
	}
	//��ÿ��autoCell��ʱ
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			((CautoCell*)pautoCellList[i])->countTime(dt);
		}
	}
	//ÿ��autoCell�䶯����ë��ת
	{
		int nautoCell=(int)pautoCellList.size();
		for(int i=0;i<nautoCell;i++){
			if(((CautoCell*)pautoCellList[i])->visible)((CautoCell*)pautoCellList[i])->wri(t);
			if(((CautoCell*)pautoCellList[i])->visible)((CautoCell*)pautoCellList[i])->updateFlglr(t);
		}
	}
	
	//---------------------�����˶�---------------------------------------------
	//proteinת��
	{
		int n=(int)pproteinList.size();
		for(int i=0;i<n;i++){
			Cprotein&protein=*pproteinList[i];
			protein.rotloc100(0.25*dt);
			protein.rotloc010(0.375*dt);
		}
	}
	//ˮ������
	if(water.visible)water.wave(t);
	
	//ʵ��water�����ǵĸ���Ϊ�˱�֤ˮ������ͼ����ȷ����Զ��߽���ƽ��
	float water_to_cell[4];
	float waterpos[4];
	water.getPosInFather(waterpos);
	waterpos[Y]=water.Horizon;
	sub(mycellpos,waterpos,water_to_cell);
	//����ƽ������
	float dx;
	float dz;
	dx=water_to_cell[X];
	dz=water_to_cell[Z];
	water.movRect(dx,dz);
	
	//-------------------����----------------------------------------------------

	//����
	if(aKeyDown){
		if(!pcell->jump){//���û���𣬲ſ�����
			pcell->startJump(0.15);
		}
	}
	//���house�컨�壬house���棬���棬ˮ�������߶�
	float houseCeilNorm[4];
	float houseCeilH=house.getH_ceil(mycellpos,pcell->Rc,houseCeilNorm);
	float groundHorizon=ground.getH(mycellpos[X],mycellpos[Z]);
	float houseFloorNorm[4];
	float houseHorizon=house.getH_floor(mycellpos,pcell->Rc,houseFloorNorm);
	float waterHorizon=water.Horizon;
	//pcell����Ӧ����ĸ߶�
	float H0=3.0;
	//pcell��������
	if(pcell->jump){
		pcell->fall(H0,houseCeilH,houseCeilNorm,houseHorizon,groundHorizon,waterHorizon,dt);
	}
	//��pcell�ĸ߶ȵ���Ϊmax(house����,����,ˮ��)
	if(pcell->jump==false){//������ʱ�����ڵ���
		pcell->adjustHeightToSurface(houseHorizon,houseFloorNorm,groundHorizon,waterHorizon,H0,dt);
	}

	//pcell��ɫ
	pcell->changeCoreColor(t);
	
	//pcell������ʱ�Ӹ���
	pcell->timerscount(dt);
	
	//pcell�䶯
	pcell->wri(t);
	//pcell��ë��ת
	pcell->updateFlglr(t);
	
	//�����ڵ�
	if(dKeyDown){
		if(pcell->fire(t)){//ȷʵ����Ų�������
			alSource3f(source_gun_small, AL_POSITION, mycellpos[X],mycellpos[Y],mycellpos[Z]);
			alSourcePlay(source_gun_small);
		}
		
	}
	
	
	//�ڵ��˶�
	pcell->cannonballsMov(dt);
	//----------------------------------------------*pcell�ƶ�
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
	//���������ƶ�ʱ������
	{
		//���source_move_on_ground��source_move_on_water�Ĳ���״̬
		ALint state_move_on_ground;
		ALint state_move_on_water;
		alGetSourcei(source_move_on_ground, AL_SOURCE_STATE,&state_move_on_ground);
		alGetSourcei(source_move_on_water, AL_SOURCE_STATE,&state_move_on_water);
		int onWhich=pcell->topSurface(houseHorizon,groundHorizon,waterHorizon);
		if(pcell->bmoving==true&&pcell->jump==false){//����ƶ���δ����
			if(onWhich==1||onWhich==2){//����ڵ����house����
				if(state_move_on_water==AL_PLAYING){//��ͣsource_move_on_water
					alSourcePause(source_move_on_water);
					//���Ź�����
					alSource3f(source_water_gulu,AL_POSITION,mycellpos[X],waterHorizon,mycellpos[Z]);
					alSourcePlay(source_water_gulu);
				}
				if(state_move_on_ground!= AL_PLAYING){//���source_move_on_ground��ֹͣ�����ٴ�����
					alSourcePlay(source_move_on_ground);
				}
				//������Դλ��
				alSource3f(source_move_on_ground,AL_POSITION,mycellpos[X],mycellpos[Y],mycellpos[Z]);
			}else if(onWhich==0){//�����ˮ��
				if(state_move_on_ground==AL_PLAYING)alSourcePause(source_move_on_ground);//��ͣsource_move_on_ground
				//���source_move_on_water��״̬
				if(state_move_on_water!= AL_PLAYING){//���source_move_on_water��ֹͣ�����ٴ�����
					alSourcePlay(source_move_on_water);
				}
				//������Դλ��
				alSource3f(source_move_on_water,AL_POSITION,mycellpos[X],mycellpos[Y],mycellpos[Z]);
			}
		}else{//�������
			if(state_move_on_ground==AL_PLAYING)alSourcePause(source_move_on_ground);//��ͣsource_move_on_ground
			if(state_move_on_water==AL_PLAYING){//��ͣsource_move_on_water
				alSourcePause(source_move_on_water);
				//���Ź�����
				alSource3f(source_water_gulu,AL_POSITION,mycellpos[X],waterHorizon,mycellpos[Z]);
				alSourcePlay(source_water_gulu);
			}
		}
	}
	//------------------------------*pcell�ƶ���ִ�����뾲̬�������ײ��⣬�����ײ����ָ�
	//ȡ��pcell�ƶ����������
	pcell->getPosInFather(mycellpos);
	//pcell��house����ײ
	float e_back[4];
	float up[4];//����ռ��е���������
	veccopy(pcell->RMmat+4,up);//�ȼ���pcell->toFather(0,1,0,0,up)���õ�up
	float d=house.collisionTestWithWall_multiPoint(mycellpos,pcell->Rc,up,e_back);
	if(d>0){
		//�ָ�
		matcopy(pcell->RMmatStore,pcell->RMmat);
		float back[4];
		mul(d*0.15,e_back,back);
		back[Y]=0;
		pcell->movloc_inFather(back[X],back[Y],back[Z]);
	}
	
	
	//----------------------------------------------------*pcell��ת
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
	//-------------������������ƻؽ���
	movInBond(width_mov,height_mov,dt,t);
	//-------------ɾ����������
	ridDiedThings();
	//-------------����������
	rebrithThings(t);
	//-----------------------------����LOD�ȼ�����߿��
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
