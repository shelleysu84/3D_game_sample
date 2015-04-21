//一部分数学运算(sse版本)
inline void prodm_general( const float b[16],const float a[16],float r[16])//r可以是a,b之一
{
	__asm{
		mov esi,b //b[i]=[esi+04h*i]
		mov edx,a //a[i]=[edx+04h*i]
		//用xmm0~xmm3存放b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		movups xmm3,[esi+04h*12]//b[12~15]->xmm3
		//--------------------计算r[0~3],结果存到xmm4（用xmm5作中转）
		//r[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]+a[3]*b[12];
		//r[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]+a[3]*b[13];
		//r[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]+a[3]*b[14];
		//r[3]=a[0]*b[3]+a[1]*b[7]+a[2]*b[11]+a[3]*b[15];
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//得到xmm4
		mulps xmm4,xmm0//第一列乘积存到xmm4
		//计算第二列乘积
		//使xmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm1//第二列乘积存到xmm5
		addps xmm4,xmm5//第二列乘积累加到xmm4
		//计算第三列乘积
		//使xmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm2//第三列乘积存到xmm5
		addps xmm4,xmm5//第三列乘积累加到xmm4
		//计算第四列乘积
		//使xmm5=(a[3],a[3],a[3],a[3])
		movss xmm5,[edx+04h*3]//a[3]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm3//第四列乘积存到xmm5
		addps xmm4,xmm5//第四列乘积累加到xmm4（得到xmm4）
		//--------------------计算r[4~7],结果存到xmm5（用xmm6作中转）
		//r[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]+a[7]*b[12];
		//r[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]+a[7]*b[13];
		//r[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]+a[7]*b[14];
		//r[7]=a[4]*b[3]+a[5]*b[7]+a[6]*b[11]+a[7]*b[15];
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm0//第一列乘积存到xmm5
		//计算第二列乘积
		//使xmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm1//第二列乘积存到xmm6
		addps xmm5,xmm6//第二列乘积累加到xmm5
		//计算第三列乘积
		//使xmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm2//第三列乘积存到xmm6
		addps xmm5,xmm6//第三列乘积累加到xmm5
		//计算第四列乘积
		//使xmm6=(a[7],a[7],a[7],a[7])
		movss xmm6,[edx+04h*7]//a[7]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm3//第四列乘积存到xmm6
		addps xmm5,xmm6//第四列乘积累加到xmm5（得到xmm5）
		//--------------------计算r[8~11],结果存到xmm6（用xmm7作中转）
		//r[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]+a[11]*b[12];
		//r[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]+a[11]*b[13];
		//r[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]+a[11]*b[14];
		//r[11]=a[8]*b[3]+a[9]*b[7]+a[10]*b[11]+a[11]*b[15];
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm0//第一列乘积存到xmm6
		//计算第二列乘积
		//使xmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm1//第二列乘积存到xmm7
		addps xmm6,xmm7//第二列乘积累加到xmm6
		//计算第三列乘积
		//使xmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm2//第三列乘积存到xmm7
		addps xmm6,xmm7//第三列乘积累加到xmm6
		//计算第四列乘积
		//使xmm7=(a[11],a[11],a[11],a[11])
		movss xmm7,[edx+04h*11]//a[11]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm3//第四列乘积存到xmm7
		addps xmm6,xmm7//第四列乘积累加到xmm6（得到xmm6）
		//--------------------计算r[12~15],结果存到xmm7（用xmm0作中转）
		//注：虽然现在xmm0中存着b[0~3]，所以还不能用于中转，但是下面我们将看到
		//在计算完第一列乘积之后，xmm0就腾出来了。而且计算第一列乘积并不需要xmm0作中转
		//r[12]=a[12]*b[0]+a[13]*b[4]+a[14]*b[8]+a[15]*b[12];
		//r[13]=a[12]*b[1]+a[13]*b[5]+a[14]*b[9]+a[15]*b[13];
		//r[14]=a[12]*b[2]+a[13]*b[6]+a[14]*b[10]+a[15]*b[14];
		//r[15]=a[12]*b[3]+a[13]*b[7]+a[14]*b[11]+a[15]*b[15];
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm7=(a[12],a[12],a[12],a[12])
		movss xmm7, [edx+04h*12]//a[12]->xmm7[0]
		shufps xmm7, xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm0//第一列乘积存到xmm7（此时xmm0已腾出，下面可以用xmm0中转了）
		//计算第二列乘积
		//使xmm0=(a[13],a[13],a[13],a[13])
		movss xmm0, [edx+04h*13]//a[13]->xmm0[0]
		shufps xmm0,xmm0,_MM_SHUFFLE(0,0,0,0)//得到xmm0
		mulps xmm0,xmm1//第二列乘积存到xmm0
		addps xmm7,xmm0//第二列乘积累加到xmm7
		//计算第三列乘积
		//使xmm0=(a[14],a[14],a[14],a[14])
		movss xmm0,[edx+04h*14]//a[14]->xmm0[0]
		shufps xmm0,xmm0,_MM_SHUFFLE(0,0,0,0)//得到xmm0
		mulps xmm0,xmm2//第三列乘积存到xmm0
		addps xmm7,xmm0//第三列乘积累加到xmm7
		//计算第四列乘积
		//使xmm0=(a[15],a[15],a[15],a[15])
		movss xmm0,[edx+04h*15]//a[15]->xmm0[0]
		shufps xmm0,xmm0,_MM_SHUFFLE(0,0,0,0)//得到xmm0
		mulps xmm0,xmm3//第四列乘积存到xmm0
		addps xmm7,xmm0//第四列乘积累加到xmm7（得到xmm7）
		//--------------将xmm4,xmm5,xmm6,xmm7分别传到r[0~3],r[4~7],r[8~11].r[12~15]
		//注意，必须在xmm4,xmm5,xmm6,xmm7全部都得到后一次性写入r，原因是r可能为a,b之一
		//如果提前修改了r（即相当于修改了a,b）会使结果错误
		mov edi,r
		movups [edi],xmm4
		movups [edi+04h*4],xmm5
		movups [edi+04h*8],xmm6
		movups [edi+04h*12],xmm7//得到r
	}
}


inline void prodm_lastRowIs0001( const float b[16],const float a[16],float r[16])
//r可以是a,b之一
//其中a,b均为
// a d g x 
// b e h y 
// c f i z 
// 0 0 0 1  
//型矩阵
{
	__asm{
		mov esi,b //b[i]=[esi+04h*i]
		mov edx,a //a[i]=[edx+04h*i]
		//用xmm0~xmm2存放b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		//--------------------计算r[0~3],结果存到xmm4（用xmm5作中转）
		//r[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]
		//r[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]
		//r[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]
		//r[3]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//得到xmm4
		mulps xmm4,xmm0//第一列乘积存到xmm4
		//计算第二列乘积
		//使xmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm1//第二列乘积存到xmm5
		addps xmm4,xmm5//第二列乘积累加到xmm4
		//计算第三列乘积
		//使xmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm2//第三列乘积存到xmm5
		addps xmm4,xmm5//第三列乘积累加到xmm4(得到xmm4)
		//--------------------计算r[4~7],结果存到xmm5（用xmm6作中转）
		//r[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]
		//r[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]
		//r[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]
		//r[7]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm0//第一列乘积存到xmm5
		//计算第二列乘积
		//使xmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm1//第二列乘积存到xmm6
		addps xmm5,xmm6//第二列乘积累加到xmm5
		//计算第三列乘积
		//使xmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm2//第三列乘积存到xmm6
		addps xmm5,xmm6//第三列乘积累加到xmm5(得到xmm5)
		//--------------------计算r[8~11],结果存到xmm6（用xmm7作中转）
		//r[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]
		//r[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]
		//r[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]
		//r[11]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm0//第一列乘积存到xmm6
		//计算第二列乘积
		//使xmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm1//第二列乘积存到xmm7
		addps xmm6,xmm7//第二列乘积累加到xmm6
		//计算第三列乘积
		//使xmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm2//第三列乘积存到xmm7
		addps xmm6,xmm7//第三列乘积累加到xmm6(得到xmm6)
		//--------------------计算r[12~15],结果存到xmm7（用xmm3作中转，xmm3目前还未使用过）
		//注意，r[12~15]表达式特殊，有第四列项，但不是乘积
		//r[12]=a[12]*b[0]+a[13]*b[4]+a[14]*b[8]+b[12];
		//r[13]=a[12]*b[1]+a[13]*b[5]+a[14]*b[9]+b[13];
		//r[14]=a[12]*b[2]+a[13]*b[6]+a[14]*b[10]+b[14];
		//r[15]=1;
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm7=(a[12],a[12],a[12],a[12])
		movss xmm7, [edx+04h*12]//a[12]->xmm7[0]
		shufps xmm7, xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm0//第一列乘积存到xmm7
		//计算第二列乘积
		//使xmm3=(a[13],a[13],a[13],a[13])
		movss xmm3, [edx+04h*13]//a[13]->xmm3[0]
		shufps xmm3,xmm3,_MM_SHUFFLE(0,0,0,0)//得到xmm3
		mulps xmm3,xmm1//第二列乘积存到xmm3
		addps xmm7,xmm3//第二列乘积累加到xmm7
		//计算第三列乘积
		//使xmm3=(a[14],a[14],a[14],a[14])
		movss xmm3,[edx+04h*14]//a[14]->xmm3[0]
		shufps xmm3,xmm3,_MM_SHUFFLE(0,0,0,0)//得到xmm3
		mulps xmm3,xmm2//第三列乘积存到xmm3
		addps xmm7,xmm3//第三列乘积累加到xmm7
		//将第四列累加到xmm7
		//将第四列数据加载到xmm3
		movups xmm3,[esi+04h*12]//b[12~15]->xmm3
		//将第四列累加到xmm7
		addps xmm7,xmm3//得到xmm7
		//--------------将xmm4,xmm5,xmm6,xmm7分别传到r[0~3],r[4~7],r[8~11].r[12~15]
		mov edi,r
		movups [edi],xmm4
		movups [edi+04h*4],xmm5
		movups [edi+04h*8],xmm6
		movups [edi+04h*12],xmm7//得到r
	}
}

inline void prodm_Rmat_Rmat( const float b[16],const float a[16],float r[16])
//b*a=r
//r可以是b,a之一
//其中b和a均为旋转矩阵
{

	// 0 4 8  12
	// 1 5 9  13
	// 2 6 10 14
	// 3 7 11 15
	//----------------------------------
	// a' d' g' 0    a d g 0      ? ? ? 0
	// b' e' h' 0 *  b e h 0   =  ? ? ? 0
	// c' f' i' 0    e f i 0      ? ? ? 0
	// 0  0  0  1    0 0 0 1      0 0 0 1
	__asm{
		mov esi,b //b[i]=[esi+04h*i]
		mov edx,a //a[i]=[edx+04h*i]
		//用xmm0~xmm2存放b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		//--------------------计算r[0~3],结果存到xmm4（用xmm5作中转）
		//r[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]
		//r[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]
		//r[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]
		//r[3]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//得到xmm4
		mulps xmm4,xmm0//第一列乘积存到xmm4
		//计算第二列乘积
		//使xmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm1//第二列乘积存到xmm5
		addps xmm4,xmm5//第二列乘积累加到xmm4
		//计算第三列乘积
		//使xmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm2//第三列乘积存到xmm5
		addps xmm4,xmm5//第三列乘积累加到xmm4(得到xmm4)
		//--------------------计算r[4~7],结果存到xmm5（用xmm6作中转）
		//r[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]
		//r[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]
		//r[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]
		//r[7]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm0//第一列乘积存到xmm5
		//计算第二列乘积
		//使xmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm1//第二列乘积存到xmm6
		addps xmm5,xmm6//第二列乘积累加到xmm5
		//计算第三列乘积
		//使xmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm2//第三列乘积存到xmm6
		addps xmm5,xmm6//第三列乘积累加到xmm5(得到xmm5)
		//--------------------计算r[8~11],结果存到xmm6（用xmm7作中转）
		//r[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]
		//r[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]
		//r[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]
		//r[11]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm0//第一列乘积存到xmm6
		//计算第二列乘积
		//使xmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm1//第二列乘积存到xmm7
		addps xmm6,xmm7//第二列乘积累加到xmm6
		//计算第三列乘积
		//使xmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm2//第三列乘积存到xmm7
		addps xmm6,xmm7//第三列乘积累加到xmm6(得到xmm6)
		//--------------将xmm4,xmm5,xmm6分别传到r[0~3],r[4~7],r[8~11]
		mov edi,r
		movups [edi],xmm4
		movups [edi+04h*4],xmm5
		movups [edi+04h*8],xmm6//得到了r的前三列
	}
	//制作r的第四列
	r[12]=0;
	r[13]=0;
    r[14]=0;
	r[15]=1;
	
}


inline void prodm_RMmat_Rmat(float b[16],const float a[16])
//b*a，结果存到b
//其中b为旋转平移矩阵，a为旋转矩阵
{
	// 0 4 8  12
	// 1 5 9  13
	// 2 6 10 14
	// 3 7 11 15
	//----------------------------------
	// a d g x    a' d' g' 0      ? ? ? x
	// b e h y *  b' e' h' 0   =  ? ? ? y
	// c f i z    e' f' i' 0      ? ? ? z
	// 0 0 0 1    0  0  0  1      0 0 0 1
	__asm{
		mov esi,b //b[i]=[esi+04h*i]
		mov edx,a //a[i]=[edx+04h*i]
		//用xmm0~xmm2存放b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		//--------------------计算b'[0~3],结果存到xmm4（用xmm5作中转）
		//b'[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]
		//b'[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]
		//b'[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]
		//b'[3]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//得到xmm4
		mulps xmm4,xmm0//第一列乘积存到xmm4
		//计算第二列乘积
		//使xmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm1//第二列乘积存到xmm5
		addps xmm4,xmm5//第二列乘积累加到xmm4
		//计算第三列乘积
		//使xmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm2//第三列乘积存到xmm5
		addps xmm4,xmm5//第三列乘积累加到xmm4(得到xmm4)
		//--------------------计算b'[4~7],结果存到xmm5（用xmm6作中转）
		//b'[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]
		//b'[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]
		//b'[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]
		//b'[7]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm0//第一列乘积存到xmm5
		//计算第二列乘积
		//使xmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm1//第二列乘积存到xmm6
		addps xmm5,xmm6//第二列乘积累加到xmm5
		//计算第三列乘积
		//使xmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm2//第三列乘积存到xmm6
		addps xmm5,xmm6//第三列乘积累加到xmm5(得到xmm5)
		//--------------------计算b'[8~11],结果存到xmm6（用xmm7作中转）
		//b'[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]
		//b'[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]
		//b'[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]
		//b'[11]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm0//第一列乘积存到xmm6
		//计算第二列乘积
		//使xmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm1//第二列乘积存到xmm7
		addps xmm6,xmm7//第二列乘积累加到xmm6
		//计算第三列乘积
		//使xmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm2//第三列乘积存到xmm7
		addps xmm6,xmm7//第三列乘积累加到xmm6(得到xmm6)
		//--------------将xmm4,xmm5,xmm6分别传到b'[0~3],b'[4~7],b'[8~11]
		movups [esi],xmm4
		movups [esi+04h*4],xmm5
		movups [esi+04h*8],xmm6//得到了b'的前三列
		//b'的第四列由b直接继承，不需要再赋值
	}
}


inline void prodm_Rmat_RMmat(const float b[16],float a[16])
//a*b，结果存到a
//其中a为旋转平移矩阵，b为旋转矩阵
{
	// 0 4 8  12
	// 1 5 9  13
	// 2 6 10 14
	// 3 7 11 15
	//----------------------------------
	// a' d' g' 0    a d g x      ? ? ? a'x+d'y+g'z
	// b' e' h' 0 *  b e h y   =  ? ? ? b'x+e'y+h'z
	// c' f' i' 0    e f i z      ? ? ? c'x+f'y+i'z
	// 0  0  0  1    0 0 0 1      0 0 0 1
	__asm{
		mov esi,b //b[i]=[esi+04h*i]
		mov edx,a //a[i]=[edx+04h*i]
		//用xmm0~xmm2存放b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		//--------------------计算a'[0~3],结果存到xmm4（用xmm5作中转）
		//a'[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]
		//a'[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]
		//a'[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]
		//a'[3]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//得到xmm4
		mulps xmm4,xmm0//第一列乘积存到xmm4
		//计算第二列乘积
		//使xmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm1//第二列乘积存到xmm5
		addps xmm4,xmm5//第二列乘积累加到xmm4
		//计算第三列乘积
		//使xmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm2//第三列乘积存到xmm5
		addps xmm4,xmm5//第三列乘积累加到xmm4(得到xmm4)
		//--------------------计算a'[4~7],结果存到xmm5（用xmm6作中转）
		//a'[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]
		//a'[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]
		//a'[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]
		//a'[7]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//得到xmm5
		mulps xmm5,xmm0//第一列乘积存到xmm5
		//计算第二列乘积
		//使xmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm1//第二列乘积存到xmm6
		addps xmm5,xmm6//第二列乘积累加到xmm5
		//计算第三列乘积
		//使xmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm2//第三列乘积存到xmm6
		addps xmm5,xmm6//第三列乘积累加到xmm5(得到xmm5)
		//--------------------计算a'[8~11],结果存到xmm6（用xmm7作中转）
		//a'[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]
		//a'[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]
		//a'[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]
		//a'[11]=0
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//得到xmm6
		mulps xmm6,xmm0//第一列乘积存到xmm6
		//计算第二列乘积
		//使xmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm1//第二列乘积存到xmm7
		addps xmm6,xmm7//第二列乘积累加到xmm6
		//计算第三列乘积
		//使xmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm2//第三列乘积存到xmm7
		addps xmm6,xmm7//第三列乘积累加到xmm6(得到xmm6)
		//--------------------计算a'[12~15],结果存到xmm7（用xmm3作中转，xmm3目前还未使用过）
		//a'[12]=a[12]*b[0]+a[13]*b[4]+a[14]*b[8]
		//a'[13]=a[12]*b[1]+a[13]*b[5]+a[14]*b[9]
		//a'[14]=a[12]*b[2]+a[13]*b[6]+a[14]*b[10]
		//a'[15]=1;
		//注意，a'[15]=a[12]*b[3]+a[13]*b[7]+a[14]*b[11]+a[15]*b[15]=1
		//但由于我们这里为了减少计算量省略了最后一列（第四列）乘积项，
		//因此我们计算a'[15]实际用的是a[12]*b[3]+a[13]*b[7]+a[14]*b[11]（它必定等于0）
		//所以我们必须先记着a'[15]的值是不对的，等最后要将它修正为1
		//计算第一列乘积（第一列乘积不用中转）
		//使xmm7=(a[12],a[12],a[12],a[12])
		movss xmm7, [edx+04h*12]//a[12]->xmm7[0]
		shufps xmm7, xmm7,_MM_SHUFFLE(0,0,0,0)//得到xmm7
		mulps xmm7,xmm0//第一列乘积存到xmm7
		//计算第二列乘积
		//使xmm3=(a[13],a[13],a[13],a[13])
		movss xmm3, [edx+04h*13]//a[13]->xmm3[0]
		shufps xmm3,xmm3,_MM_SHUFFLE(0,0,0,0)//得到xmm3
		mulps xmm3,xmm1//第二列乘积存到xmm3
		addps xmm7,xmm3//第二列乘积累加到xmm7
		//计算第三列乘积
		//使xmm3=(a[14],a[14],a[14],a[14])
		movss xmm3,[edx+04h*14]//a[14]->xmm3[0]
		shufps xmm3,xmm3,_MM_SHUFFLE(0,0,0,0)//得到xmm3
		mulps xmm3,xmm2//第三列乘积存到xmm3
		addps xmm7,xmm3//第三列乘积累加到xmm7
		//--------------将xmm4,xmm5,xmm6,xmm7分别传到a'[0~3],a'[4~7],a'[8~11],a'[12~15]
		movups [edx],xmm4
		movups [edx+04h*4],xmm5
		movups [edx+04h*8],xmm6
		movups [edx+04h*12],xmm7//得到a'
	
	}
	//将a'[15]修正为1
	a[15]=1;
}//*/