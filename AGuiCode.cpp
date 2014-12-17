#include "StdAfx.h"
#include ".\aguicode.h"

CAGuiCode::CAGuiCode(void)
{
}

CAGuiCode::~CAGuiCode(void)
{
}

void CAGuiCode::Transform(bool *Out, bool *In, const char *Table, int len) //通用置换函数 
{ 
	for(int i = 0; i < len; ++ i) 
		Tmp[i] = In[ Table[i] - 1 ]; 
	memcpy(Out, Tmp, len); 
} 


void CAGuiCode::Xor(bool *InA, const bool *InB, int len) //异或运算 
{ 
	for(int i = 0; i < len; ++ i) 
		InA[i] ^= InB[i]; 
} 


void CAGuiCode::MoveLeft(bool *In, int len, int loop) //循环左移 
{ 
	memcpy(Tmp, In, loop); 
	memcpy(In, In + loop, len - loop); 
	memcpy(In + len - loop, Tmp, loop); 
} 


void CAGuiCode::Byte2Bit(bool *Out, const char *In, int bits) //字节转换成位 
{ 
	for(int i = 0; i < bits; ++ i) 
		Out[i] = (In[i >> 3] >> (i & 7)) & 1; 
} 


void CAGuiCode::Bit2Byte(char *Out, const bool *In, int bits) //位转换字节 
{ 
	memset(Out, 0, bits >> 3); 
	for(int i = 0; i < bits; ++ i) 
		Out[i >> 3] |= In[i] << (i & 7); 
} 


void CAGuiCode::funS(bool Out[32], const bool In[48]) //S 盒置换 
{ 
	for(char i = 0, j, k; i < 8; ++ i, In += 6, Out += 4) 
	{ 
		j = (In[0] << 1) + In[5]; 
		k = (In[1] << 3) + (In[2] << 2) + (In[3] << 1) + In[4]; 
		Byte2Bit(Out, &Box_S[i][j][k], 4); 
	} 
} 


void CAGuiCode::funF(bool In[32], const bool Ki[48]) //F 函数 
{ 
	static bool MR[48]; 
	Transform(MR, In, Table_E, 48); 
	Xor(MR, Ki, 48); 
	funS(In, MR); 
	Transform(In, In, Table_P, 32); 
} 


void CAGuiCode::MakeSubKey(PSubKey pSubKey, const char Key[8]) //生成子密钥
{ 
	static bool K[64], *KL = &K[0], *KR = &K[28]; 
	Byte2Bit(K, Key, 64); 
	Transform(K, K, Table_PC1, 56); 
	for(int i = 0; i < 16; ++ i) 
	{ 
		MoveLeft(KL, 28, Table_Moveleft[i]); 
		MoveLeft(KR, 28, Table_Moveleft[i]); 
		Transform((*pSubKey)[i], K, Table_PC2, 48); 
	} 
} 


void CAGuiCode::MakeKey(const char* Key, int len) //生成密钥 
{ 
	memcpy(deskey, Key, len > 8 ? 8 : len); //把八位密钥考入到deskey中
	MakeSubKey(&SubKey, &deskey[0]); 
} 

void CAGuiCode::DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type) //DES加密解密
{ 
	static bool M[64], tmp[32], *Li = &M[0], *Ri = &M[32]; 
	Byte2Bit(M, In, 64); 
	Transform(M, M, Table_IP, 64); 
	if( Type == ENCRYPT )
	{ 
		for(int i = 0; i < 16; ++ i) 
		{ 
			memcpy(tmp, Ri, 32); 
			funF(Ri, (*pSubKey)[i]); 
			Xor(Ri, Li, 32); 
			memcpy(Li, tmp, 32); 
		} 
	} 

	else 
	{ 
		for(int i = 15; i >= 0; -- i) 
		{ 
			memcpy(tmp, Li, 32); 
			funF(Li, (*pSubKey)[i]); 
			Xor(Li, Ri, 32); 
			memcpy(Ri, tmp, 32); 
		} 

	} 

	Transform(M, M, Table_InverseIP, 64); 
	Bit2Byte(Out, M, 64); 
} 

bool CAGuiCode::DoDES(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type) 
{ 
	if( !( Out && In && Key && (datalen=((datalen + (8 - 1)) & ~(8 - 1) ) ) ))
		//datalen=((datalen + (8 - 1)) & ~(8 - 1) ) )作用是把数据对齐
		return false; 
	MakeKey(Key, keylen); 
	for(long i = 0, j = datalen >> 3; i < j; ++ i, Out += 8, In += 8) 
	{
		DES(Out, In,  &SubKey, Type); 
	}
	return true; 
} 

CString CAGuiCode::DES_ENCODE(CString strText,CString strKey)
{
	if(!strText || strText.Compare("")==0 || strText.GetLength()<1)
		return "" ;

	char buf[MAX_DATA_LENGTH];
	char key[MAX_KEY_LENGTH];
	
	memset(buf, 0, sizeof(buf));
	memset(key, 0, sizeof(key)); 
	strcpy(buf, strText.GetString()); 
	strcpy(key, strKey.GetString()); 
	DoDES(buf,buf,sizeof(buf),key, sizeof(key),ENCRYPT);
	CString strRes="";
	strRes.Format(_T("%s"),buf);

	return strRes;
}

CString CAGuiCode::DES_DECODE(CString strText,CString strKey)
{
	if(!strText || strText.Compare("")==0 || strText.GetLength()<1)
		return "" ;

	char buf[MAX_DATA_LENGTH];
	char key[MAX_KEY_LENGTH];
	
	memset(buf, 0, sizeof(buf));
	memset(key, 0, sizeof(key)); 
	strcpy(buf, strText.GetString()); 
	strcpy(key, strKey.GetString()); 
	DoDES(buf,buf,sizeof(buf),key, sizeof(key),DECRYPT);
	CString strRes="";
	strRes.Format(_T("%s"),buf);
	return strRes;
}


