#include <cstdlib>
#include <cstring>
#include <iostream>
#include <gcrypt.h>
#include <unistd.h>
using namespace std;
#define CIPHER_ALGO GCRY_CIPHER_AES128
char key[]="5eS3S37S49VBiw2u";
char iv[]="m4zZsFwqZkB34kGm";
//char iv[]="";

char content[]="Hello world!My name is HP!";
int main()
{
	cout<<"原文 : "<<strlen(content)<<endl<<content<<endl;

	size_t key_size = gcry_cipher_get_algo_keylen(CIPHER_ALGO);
	size_t block_size = gcry_cipher_get_algo_blklen(CIPHER_ALGO);
	size_t file_size = strlen(content);
	size_t block_required=file_size/block_size;
	if (file_size % block_size != 0){
	    block_required++;
	}
	cout<<"key_size : "<<key_size<<endl<<"block_size : "<<block_size<<endl<<"file_size : "<<file_size<<endl<<"block_required : "<<block_required<<endl;

	gcry_error_t cipher_err;
	gcry_cipher_hd_t cipher_hd;


	char *cipher_buffer=(char *)malloc(block_required*block_size+10);
	for(;;)
	{
	cipher_err=gcry_cipher_open(&cipher_hd, CIPHER_ALGO, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_CBC_CTS);
	cipher_err=gcry_cipher_setkey(cipher_hd,key,key_size);
	cipher_err=gcry_cipher_setiv(cipher_hd, iv, block_size);
	memset(cipher_buffer,0,block_required*block_size+10);
	memcpy(cipher_buffer,content,block_required*block_size);
	cipher_err=gcry_cipher_encrypt(cipher_hd,cipher_buffer,block_required*block_size,NULL,0);//加密
	cout<<"密文 : "<<strlen(cipher_buffer)<<endl<<cipher_buffer<<endl;

	cipher_err=gcry_cipher_setiv(cipher_hd, iv, block_size);//加密之后需要重新设置iv才能解密，因为iv是一次性的
	size_t file_size_out = strlen(cipher_buffer);

	cipher_err=gcry_cipher_decrypt(cipher_hd,cipher_buffer,file_size_out,NULL,0);


	cout<<"解密 : "<<strlen(cipher_buffer)<<endl<<cipher_buffer<<endl;
	gcry_cipher_close(cipher_hd);

	sleep(2);
	cout<<"---------------------------------------"<<endl;

	}
	return 0;
}