//һ������ѧ����(sse�汾)
inline void prodm_general( const float b[16],const float a[16],float r[16])//r������a,b֮һ
{
	__asm{
		mov esi,b //b[i]=[esi+04h*i]
		mov edx,a //a[i]=[edx+04h*i]
		//��xmm0~xmm3���b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		movups xmm3,[esi+04h*12]//b[12~15]->xmm3
		//--------------------����r[0~3],����浽xmm4����xmm5����ת��
		//r[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]+a[3]*b[12];
		//r[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]+a[3]*b[13];
		//r[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]+a[3]*b[14];
		//r[3]=a[0]*b[3]+a[1]*b[7]+a[2]*b[11]+a[3]*b[15];
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//�õ�xmm4
		mulps xmm4,xmm0//��һ�г˻��浽xmm4
		//����ڶ��г˻�
		//ʹxmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm1//�ڶ��г˻��浽xmm5
		addps xmm4,xmm5//�ڶ��г˻��ۼӵ�xmm4
		//��������г˻�
		//ʹxmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm2//�����г˻��浽xmm5
		addps xmm4,xmm5//�����г˻��ۼӵ�xmm4
		//��������г˻�
		//ʹxmm5=(a[3],a[3],a[3],a[3])
		movss xmm5,[edx+04h*3]//a[3]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm3//�����г˻��浽xmm5
		addps xmm4,xmm5//�����г˻��ۼӵ�xmm4���õ�xmm4��
		//--------------------����r[4~7],����浽xmm5����xmm6����ת��
		//r[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]+a[7]*b[12];
		//r[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]+a[7]*b[13];
		//r[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]+a[7]*b[14];
		//r[7]=a[4]*b[3]+a[5]*b[7]+a[6]*b[11]+a[7]*b[15];
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm0//��һ�г˻��浽xmm5
		//����ڶ��г˻�
		//ʹxmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm1//�ڶ��г˻��浽xmm6
		addps xmm5,xmm6//�ڶ��г˻��ۼӵ�xmm5
		//��������г˻�
		//ʹxmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm2//�����г˻��浽xmm6
		addps xmm5,xmm6//�����г˻��ۼӵ�xmm5
		//��������г˻�
		//ʹxmm6=(a[7],a[7],a[7],a[7])
		movss xmm6,[edx+04h*7]//a[7]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm3//�����г˻��浽xmm6
		addps xmm5,xmm6//�����г˻��ۼӵ�xmm5���õ�xmm5��
		//--------------------����r[8~11],����浽xmm6����xmm7����ת��
		//r[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]+a[11]*b[12];
		//r[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]+a[11]*b[13];
		//r[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]+a[11]*b[14];
		//r[11]=a[8]*b[3]+a[9]*b[7]+a[10]*b[11]+a[11]*b[15];
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm0//��һ�г˻��浽xmm6
		//����ڶ��г˻�
		//ʹxmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm1//�ڶ��г˻��浽xmm7
		addps xmm6,xmm7//�ڶ��г˻��ۼӵ�xmm6
		//��������г˻�
		//ʹxmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm2//�����г˻��浽xmm7
		addps xmm6,xmm7//�����г˻��ۼӵ�xmm6
		//��������г˻�
		//ʹxmm7=(a[11],a[11],a[11],a[11])
		movss xmm7,[edx+04h*11]//a[11]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm3//�����г˻��浽xmm7
		addps xmm6,xmm7//�����г˻��ۼӵ�xmm6���õ�xmm6��
		//--------------------����r[12~15],����浽xmm7����xmm0����ת��
		//ע����Ȼ����xmm0�д���b[0~3]�����Ի�����������ת�������������ǽ�����
		//�ڼ������һ�г˻�֮��xmm0���ڳ����ˡ����Ҽ����һ�г˻�������Ҫxmm0����ת
		//r[12]=a[12]*b[0]+a[13]*b[4]+a[14]*b[8]+a[15]*b[12];
		//r[13]=a[12]*b[1]+a[13]*b[5]+a[14]*b[9]+a[15]*b[13];
		//r[14]=a[12]*b[2]+a[13]*b[6]+a[14]*b[10]+a[15]*b[14];
		//r[15]=a[12]*b[3]+a[13]*b[7]+a[14]*b[11]+a[15]*b[15];
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm7=(a[12],a[12],a[12],a[12])
		movss xmm7, [edx+04h*12]//a[12]->xmm7[0]
		shufps xmm7, xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm0//��һ�г˻��浽xmm7����ʱxmm0���ڳ������������xmm0��ת�ˣ�
		//����ڶ��г˻�
		//ʹxmm0=(a[13],a[13],a[13],a[13])
		movss xmm0, [edx+04h*13]//a[13]->xmm0[0]
		shufps xmm0,xmm0,_MM_SHUFFLE(0,0,0,0)//�õ�xmm0
		mulps xmm0,xmm1//�ڶ��г˻��浽xmm0
		addps xmm7,xmm0//�ڶ��г˻��ۼӵ�xmm7
		//��������г˻�
		//ʹxmm0=(a[14],a[14],a[14],a[14])
		movss xmm0,[edx+04h*14]//a[14]->xmm0[0]
		shufps xmm0,xmm0,_MM_SHUFFLE(0,0,0,0)//�õ�xmm0
		mulps xmm0,xmm2//�����г˻��浽xmm0
		addps xmm7,xmm0//�����г˻��ۼӵ�xmm7
		//��������г˻�
		//ʹxmm0=(a[15],a[15],a[15],a[15])
		movss xmm0,[edx+04h*15]//a[15]->xmm0[0]
		shufps xmm0,xmm0,_MM_SHUFFLE(0,0,0,0)//�õ�xmm0
		mulps xmm0,xmm3//�����г˻��浽xmm0
		addps xmm7,xmm0//�����г˻��ۼӵ�xmm7���õ�xmm7��
		//--------------��xmm4,xmm5,xmm6,xmm7�ֱ𴫵�r[0~3],r[4~7],r[8~11].r[12~15]
		//ע�⣬������xmm4,xmm5,xmm6,xmm7ȫ�����õ���һ����д��r��ԭ����r����Ϊa,b֮һ
		//�����ǰ�޸���r�����൱���޸���a,b����ʹ�������
		mov edi,r
		movups [edi],xmm4
		movups [edi+04h*4],xmm5
		movups [edi+04h*8],xmm6
		movups [edi+04h*12],xmm7//�õ�r
	}
}


inline void prodm_lastRowIs0001( const float b[16],const float a[16],float r[16])
//r������a,b֮һ
//����a,b��Ϊ
// a d g x 
// b e h y 
// c f i z 
// 0 0 0 1  
//�;���
{
	__asm{
		mov esi,b //b[i]=[esi+04h*i]
		mov edx,a //a[i]=[edx+04h*i]
		//��xmm0~xmm2���b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		//--------------------����r[0~3],����浽xmm4����xmm5����ת��
		//r[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]
		//r[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]
		//r[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]
		//r[3]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//�õ�xmm4
		mulps xmm4,xmm0//��һ�г˻��浽xmm4
		//����ڶ��г˻�
		//ʹxmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm1//�ڶ��г˻��浽xmm5
		addps xmm4,xmm5//�ڶ��г˻��ۼӵ�xmm4
		//��������г˻�
		//ʹxmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm2//�����г˻��浽xmm5
		addps xmm4,xmm5//�����г˻��ۼӵ�xmm4(�õ�xmm4)
		//--------------------����r[4~7],����浽xmm5����xmm6����ת��
		//r[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]
		//r[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]
		//r[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]
		//r[7]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm0//��һ�г˻��浽xmm5
		//����ڶ��г˻�
		//ʹxmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm1//�ڶ��г˻��浽xmm6
		addps xmm5,xmm6//�ڶ��г˻��ۼӵ�xmm5
		//��������г˻�
		//ʹxmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm2//�����г˻��浽xmm6
		addps xmm5,xmm6//�����г˻��ۼӵ�xmm5(�õ�xmm5)
		//--------------------����r[8~11],����浽xmm6����xmm7����ת��
		//r[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]
		//r[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]
		//r[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]
		//r[11]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm0//��һ�г˻��浽xmm6
		//����ڶ��г˻�
		//ʹxmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm1//�ڶ��г˻��浽xmm7
		addps xmm6,xmm7//�ڶ��г˻��ۼӵ�xmm6
		//��������г˻�
		//ʹxmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm2//�����г˻��浽xmm7
		addps xmm6,xmm7//�����г˻��ۼӵ�xmm6(�õ�xmm6)
		//--------------------����r[12~15],����浽xmm7����xmm3����ת��xmm3Ŀǰ��δʹ�ù���
		//ע�⣬r[12~15]���ʽ���⣬�е�����������ǳ˻�
		//r[12]=a[12]*b[0]+a[13]*b[4]+a[14]*b[8]+b[12];
		//r[13]=a[12]*b[1]+a[13]*b[5]+a[14]*b[9]+b[13];
		//r[14]=a[12]*b[2]+a[13]*b[6]+a[14]*b[10]+b[14];
		//r[15]=1;
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm7=(a[12],a[12],a[12],a[12])
		movss xmm7, [edx+04h*12]//a[12]->xmm7[0]
		shufps xmm7, xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm0//��һ�г˻��浽xmm7
		//����ڶ��г˻�
		//ʹxmm3=(a[13],a[13],a[13],a[13])
		movss xmm3, [edx+04h*13]//a[13]->xmm3[0]
		shufps xmm3,xmm3,_MM_SHUFFLE(0,0,0,0)//�õ�xmm3
		mulps xmm3,xmm1//�ڶ��г˻��浽xmm3
		addps xmm7,xmm3//�ڶ��г˻��ۼӵ�xmm7
		//��������г˻�
		//ʹxmm3=(a[14],a[14],a[14],a[14])
		movss xmm3,[edx+04h*14]//a[14]->xmm3[0]
		shufps xmm3,xmm3,_MM_SHUFFLE(0,0,0,0)//�õ�xmm3
		mulps xmm3,xmm2//�����г˻��浽xmm3
		addps xmm7,xmm3//�����г˻��ۼӵ�xmm7
		//���������ۼӵ�xmm7
		//�����������ݼ��ص�xmm3
		movups xmm3,[esi+04h*12]//b[12~15]->xmm3
		//���������ۼӵ�xmm7
		addps xmm7,xmm3//�õ�xmm7
		//--------------��xmm4,xmm5,xmm6,xmm7�ֱ𴫵�r[0~3],r[4~7],r[8~11].r[12~15]
		mov edi,r
		movups [edi],xmm4
		movups [edi+04h*4],xmm5
		movups [edi+04h*8],xmm6
		movups [edi+04h*12],xmm7//�õ�r
	}
}

inline void prodm_Rmat_Rmat( const float b[16],const float a[16],float r[16])
//b*a=r
//r������b,a֮һ
//����b��a��Ϊ��ת����
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
		//��xmm0~xmm2���b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		//--------------------����r[0~3],����浽xmm4����xmm5����ת��
		//r[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]
		//r[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]
		//r[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]
		//r[3]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//�õ�xmm4
		mulps xmm4,xmm0//��һ�г˻��浽xmm4
		//����ڶ��г˻�
		//ʹxmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm1//�ڶ��г˻��浽xmm5
		addps xmm4,xmm5//�ڶ��г˻��ۼӵ�xmm4
		//��������г˻�
		//ʹxmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm2//�����г˻��浽xmm5
		addps xmm4,xmm5//�����г˻��ۼӵ�xmm4(�õ�xmm4)
		//--------------------����r[4~7],����浽xmm5����xmm6����ת��
		//r[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]
		//r[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]
		//r[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]
		//r[7]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm0//��һ�г˻��浽xmm5
		//����ڶ��г˻�
		//ʹxmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm1//�ڶ��г˻��浽xmm6
		addps xmm5,xmm6//�ڶ��г˻��ۼӵ�xmm5
		//��������г˻�
		//ʹxmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm2//�����г˻��浽xmm6
		addps xmm5,xmm6//�����г˻��ۼӵ�xmm5(�õ�xmm5)
		//--------------------����r[8~11],����浽xmm6����xmm7����ת��
		//r[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]
		//r[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]
		//r[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]
		//r[11]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm0//��һ�г˻��浽xmm6
		//����ڶ��г˻�
		//ʹxmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm1//�ڶ��г˻��浽xmm7
		addps xmm6,xmm7//�ڶ��г˻��ۼӵ�xmm6
		//��������г˻�
		//ʹxmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm2//�����г˻��浽xmm7
		addps xmm6,xmm7//�����г˻��ۼӵ�xmm6(�õ�xmm6)
		//--------------��xmm4,xmm5,xmm6�ֱ𴫵�r[0~3],r[4~7],r[8~11]
		mov edi,r
		movups [edi],xmm4
		movups [edi+04h*4],xmm5
		movups [edi+04h*8],xmm6//�õ���r��ǰ����
	}
	//����r�ĵ�����
	r[12]=0;
	r[13]=0;
    r[14]=0;
	r[15]=1;
	
}


inline void prodm_RMmat_Rmat(float b[16],const float a[16])
//b*a������浽b
//����bΪ��תƽ�ƾ���aΪ��ת����
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
		//��xmm0~xmm2���b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		//--------------------����b'[0~3],����浽xmm4����xmm5����ת��
		//b'[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]
		//b'[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]
		//b'[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]
		//b'[3]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//�õ�xmm4
		mulps xmm4,xmm0//��һ�г˻��浽xmm4
		//����ڶ��г˻�
		//ʹxmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm1//�ڶ��г˻��浽xmm5
		addps xmm4,xmm5//�ڶ��г˻��ۼӵ�xmm4
		//��������г˻�
		//ʹxmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm2//�����г˻��浽xmm5
		addps xmm4,xmm5//�����г˻��ۼӵ�xmm4(�õ�xmm4)
		//--------------------����b'[4~7],����浽xmm5����xmm6����ת��
		//b'[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]
		//b'[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]
		//b'[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]
		//b'[7]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm0//��һ�г˻��浽xmm5
		//����ڶ��г˻�
		//ʹxmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm1//�ڶ��г˻��浽xmm6
		addps xmm5,xmm6//�ڶ��г˻��ۼӵ�xmm5
		//��������г˻�
		//ʹxmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm2//�����г˻��浽xmm6
		addps xmm5,xmm6//�����г˻��ۼӵ�xmm5(�õ�xmm5)
		//--------------------����b'[8~11],����浽xmm6����xmm7����ת��
		//b'[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]
		//b'[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]
		//b'[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]
		//b'[11]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm0//��һ�г˻��浽xmm6
		//����ڶ��г˻�
		//ʹxmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm1//�ڶ��г˻��浽xmm7
		addps xmm6,xmm7//�ڶ��г˻��ۼӵ�xmm6
		//��������г˻�
		//ʹxmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm2//�����г˻��浽xmm7
		addps xmm6,xmm7//�����г˻��ۼӵ�xmm6(�õ�xmm6)
		//--------------��xmm4,xmm5,xmm6�ֱ𴫵�b'[0~3],b'[4~7],b'[8~11]
		movups [esi],xmm4
		movups [esi+04h*4],xmm5
		movups [esi+04h*8],xmm6//�õ���b'��ǰ����
		//b'�ĵ�������bֱ�Ӽ̳У�����Ҫ�ٸ�ֵ
	}
}


inline void prodm_Rmat_RMmat(const float b[16],float a[16])
//a*b������浽a
//����aΪ��תƽ�ƾ���bΪ��ת����
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
		//��xmm0~xmm2���b
		movups xmm0,[esi]//b[0~3]->xmm0
		movups xmm1,[esi+04h*4]//b[4~7]->xmm1
		movups xmm2,[esi+04h*8]//b[8~11]->xmm2
		//--------------------����a'[0~3],����浽xmm4����xmm5����ת��
		//a'[0]=a[0]*b[0]+a[1]*b[4]+a[2]*b[8]
		//a'[1]=a[0]*b[1]+a[1]*b[5]+a[2]*b[9]
		//a'[2]=a[0]*b[2]+a[1]*b[6]+a[2]*b[10]
		//a'[3]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm4=(a[0],a[0],a[0],a[0])
		movss xmm4, [edx]//a[0]->xmm4[0]
		shufps xmm4, xmm4,_MM_SHUFFLE(0,0,0,0)//�õ�xmm4
		mulps xmm4,xmm0//��һ�г˻��浽xmm4
		//����ڶ��г˻�
		//ʹxmm5=(a[1],a[1],a[1],a[1])
		movss xmm5, [edx+04h]//a[1]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm1//�ڶ��г˻��浽xmm5
		addps xmm4,xmm5//�ڶ��г˻��ۼӵ�xmm4
		//��������г˻�
		//ʹxmm5=(a[2],a[2],a[2],a[2])
		movss xmm5,[edx+04h*2]//a[2]->xmm5[0]
		shufps xmm5,xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm2//�����г˻��浽xmm5
		addps xmm4,xmm5//�����г˻��ۼӵ�xmm4(�õ�xmm4)
		//--------------------����a'[4~7],����浽xmm5����xmm6����ת��
		//a'[4]=a[4]*b[0]+a[5]*b[4]+a[6]*b[8]
		//a'[5]=a[4]*b[1]+a[5]*b[5]+a[6]*b[9]
		//a'[6]=a[4]*b[2]+a[5]*b[6]+a[6]*b[10]
		//a'[7]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm5=(a[4],a[4],a[4],a[4])
		movss xmm5, [edx+04h*4]//a[4]->xmm5[0]
		shufps xmm5, xmm5,_MM_SHUFFLE(0,0,0,0)//�õ�xmm5
		mulps xmm5,xmm0//��һ�г˻��浽xmm5
		//����ڶ��г˻�
		//ʹxmm6=(a[5],a[5],a[5],a[5])
		movss xmm6, [edx+04h*5]//a[5]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm1//�ڶ��г˻��浽xmm6
		addps xmm5,xmm6//�ڶ��г˻��ۼӵ�xmm5
		//��������г˻�
		//ʹxmm6=(a[6],a[6],a[6],a[6])
		movss xmm6,[edx+04h*6]//a[6]->xmm6[0]
		shufps xmm6,xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm2//�����г˻��浽xmm6
		addps xmm5,xmm6//�����г˻��ۼӵ�xmm5(�õ�xmm5)
		//--------------------����a'[8~11],����浽xmm6����xmm7����ת��
		//a'[8]=a[8]*b[0]+a[9]*b[4]+a[10]*b[8]
		//a'[9]=a[8]*b[1]+a[9]*b[5]+a[10]*b[9]
		//a'[10]=a[8]*b[2]+a[9]*b[6]+a[10]*b[10]
		//a'[11]=0
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm6=(a[8],a[8],a[8],a[8])
		movss xmm6, [edx+04h*8]//a[8]->xmm6[0]
		shufps xmm6, xmm6,_MM_SHUFFLE(0,0,0,0)//�õ�xmm6
		mulps xmm6,xmm0//��һ�г˻��浽xmm6
		//����ڶ��г˻�
		//ʹxmm7=(a[9],a[9],a[9],a[9])
		movss xmm7, [edx+04h*9]//a[9]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm1//�ڶ��г˻��浽xmm7
		addps xmm6,xmm7//�ڶ��г˻��ۼӵ�xmm6
		//��������г˻�
		//ʹxmm7=(a[10],a[10],a[10],a[10])
		movss xmm7,[edx+04h*10]//a[10]->xmm7[0]
		shufps xmm7,xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm2//�����г˻��浽xmm7
		addps xmm6,xmm7//�����г˻��ۼӵ�xmm6(�õ�xmm6)
		//--------------------����a'[12~15],����浽xmm7����xmm3����ת��xmm3Ŀǰ��δʹ�ù���
		//a'[12]=a[12]*b[0]+a[13]*b[4]+a[14]*b[8]
		//a'[13]=a[12]*b[1]+a[13]*b[5]+a[14]*b[9]
		//a'[14]=a[12]*b[2]+a[13]*b[6]+a[14]*b[10]
		//a'[15]=1;
		//ע�⣬a'[15]=a[12]*b[3]+a[13]*b[7]+a[14]*b[11]+a[15]*b[15]=1
		//��������������Ϊ�˼��ټ�����ʡ�������һ�У������У��˻��
		//������Ǽ���a'[15]ʵ���õ���a[12]*b[3]+a[13]*b[7]+a[14]*b[11]�����ض�����0��
		//�������Ǳ����ȼ���a'[15]��ֵ�ǲ��Եģ������Ҫ��������Ϊ1
		//�����һ�г˻�����һ�г˻�������ת��
		//ʹxmm7=(a[12],a[12],a[12],a[12])
		movss xmm7, [edx+04h*12]//a[12]->xmm7[0]
		shufps xmm7, xmm7,_MM_SHUFFLE(0,0,0,0)//�õ�xmm7
		mulps xmm7,xmm0//��һ�г˻��浽xmm7
		//����ڶ��г˻�
		//ʹxmm3=(a[13],a[13],a[13],a[13])
		movss xmm3, [edx+04h*13]//a[13]->xmm3[0]
		shufps xmm3,xmm3,_MM_SHUFFLE(0,0,0,0)//�õ�xmm3
		mulps xmm3,xmm1//�ڶ��г˻��浽xmm3
		addps xmm7,xmm3//�ڶ��г˻��ۼӵ�xmm7
		//��������г˻�
		//ʹxmm3=(a[14],a[14],a[14],a[14])
		movss xmm3,[edx+04h*14]//a[14]->xmm3[0]
		shufps xmm3,xmm3,_MM_SHUFFLE(0,0,0,0)//�õ�xmm3
		mulps xmm3,xmm2//�����г˻��浽xmm3
		addps xmm7,xmm3//�����г˻��ۼӵ�xmm7
		//--------------��xmm4,xmm5,xmm6,xmm7�ֱ𴫵�a'[0~3],a'[4~7],a'[8~11],a'[12~15]
		movups [edx],xmm4
		movups [edx+04h*4],xmm5
		movups [edx+04h*8],xmm6
		movups [edx+04h*12],xmm7//�õ�a'
	
	}
	//��a'[15]����Ϊ1
	a[15]=1;
}//*/