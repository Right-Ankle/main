/* ica.h */

struct pca{	//PCAの結果を格納する構造体

	double r[64];
	double a[64][64];
	int p;

};

struct t{	//ソート用構造体
	double val;
	int num;
};

struct tt{	//DCT係数のソート用構造体 絶対値を追加
	double val;
	double abs;
	int num;
};

struct c{	//コスト関数用構造体
	int num;
	int r_num;
	double rate;
	double dis;
	double cost;
};

#define N 256
double lambda;
int Q;

void gentox(unsigned char gen[][256], double x[][1024]);
void gentox2(double gen[][256], double x[][1024]);
void new_pre(double **xx, double *avg, int data);
void new_whitening(struct pca new_pca, double **xx, double v[][64], double p[][64], int data);
void new_seki(double w[][64], double **xx, double **yy, int data);
void makew(double w[][64]);
void new_sigmoid(double **yy, double **sig, int data);
void new_ytoyt(double **yy, double **yt, int data);
void new_seki2(double **sig, double **yt, double z[][64], int data);
void identity(double z[][64], double q[][64]);
void seki3(double q[][64], double w[][64], double dw[][64]);
void normal(double w[][64]);
void inv(double w[][64], double inv_w[][64]);
void wtosai(double w[64][64], unsigned char sai[][64]);
double new_kurt(double **yy, int data, double *b_kurt);
int block_image(unsigned char origin[][256], int block[1024]);
void seki(double w[][64], double x[][1024], double y[][1024]);
void seki5(double w[][64], double x[][1024], double y[][1024]);
void seki5_Block(double w[][64], double x[][1024], double y[64], int num);
void xtogen(double x[][1024], unsigned char gen[][256], double avg[1024]);
void xtogen_Block(double x[64], unsigned char gen[64], double avg[1024], int num);
void avg_inter(unsigned char gen[][256], double avg[1024]);
void avg_inter_Block(unsigned char gen[8][8], double avg[1024], int num);
struct pca new_pca(unsigned char gen[256][256]);
void new_ica(unsigned char origin[][256], struct pca pcaTemp, double ny[][1024], double nw[][64], double navg[1024]);
double psnr(unsigned char origin[][256], unsigned char saikousei[][256]);
void ICA(unsigned char origin[][256], struct pca emp, double ny[][1024], double nw[][64], double navg[1024], int e, double f);
void ICA_temp(unsigned char origin[][256], struct pca emp, double ny[][1024], double nw[][64], double navg[1024], int e, double f, int num1, int num2);
double inner_product(double a[64], double b[64]);
void b_L0_dct(double in[64][1024], int out[1024]);
void b_L0_ica(double in[64][1024], int out[1024]);
void dct(unsigned char gen[][256], double coe[][256], int n);
void quantization(double coe[][256], int Q);
void idct(double coe[][256], unsigned char sai[][256], int n);
void idct_Block(double coe[64][1024], unsigned char sai[64], int n, int num);
void block_psnr(unsigned char origin[][256], unsigned char gen[][256], double b_psnr[1024]);
void block_mse(unsigned char origin[][256], unsigned char gen[][256], double b_mse[1024]);
void dctcoe_conv(double in[256][256], double out[64][1024]);
void mp(double y[][1024], double avg[1024], double w[][64], int mpans[][64]);
void IBS(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int block_flag[1024], double ibs[64]);
double ICAb_PSNR(unsigned char origin[][256], unsigned char sai[][256], int block_flag[1024]);
double get_entropy(double coe[][256]);
void double_idct(double coe[][N], double sai[][N], int n);
void double_dct(double gen[][256], double coe[][256], int n);
void Hybrid_image2(unsigned char ica_sai[][256], unsigned char dct_sai[][256], double zcoe[][256], int new_image_flag[1024], unsigned char result[][256]);
double Hybrid_entropy3(double coe[][256], double y[][1024], double zcoe[][256], int new_image_flag[1024]);
double basis_entropy(double w[64][64], int basis_flag[64]);
void score(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int RBS_flag[64], int Q, int block_flag[1024], double improvement[5], int num, int count, int no_flag[64]);
void segmentation(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int mpans[][64], int block_flag[1024], int Q);
void RBS(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int block_flag[1024], int Q);
void RBS_result(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int RBS_flag[64], int Q, int block_flag[1024]);

int img_read_gray(int *G, char *input, char *name, int h_size, int v_size);
void img_write_gray(int *G, char *conv, int h_size, int v_size);

void segmentation_ent(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int mpans[][64], int block_flag[1024], int Q);
void get_seg_image(unsigned char origin[][256], int b_flag[1024], int Q);

void b_entropy_ica(double y[64][1024], double b_ent_ica[1024]);
void b_entropy_dct(double coe[256][256], double b_ent_dct[1024]);

void b_var_ica(double in[64][1024], double var[1024], double ske[1024]);

void RBS_yori(unsigned char origin[][256], double y[][1024], double yori[][1024], double avg[1024], double w[][64], int block_flag[1024], int Q);
void RBS_result_yori(unsigned char origin[][256], double y[][1024], double yori[][1024], double avg[1024], double w[][64], int RBS_flag[64], int Q, int block_flag[1024]);
void RBS_result_RDout(unsigned char origin[][256], double y[][1024], double yori[][1024], double avg[1024], double w[][64], int RBS_flag[64], int Q, int block_flag[1024]);

void segmentation_RD_single(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int mpans[][64], int block_flag[1024], int QQ);
void make_cost(double rate[1024], double dis[1024], double cost[1024]);
void b_mse(unsigned char origin[][256], unsigned char gen[][256], double mse[1024]);
double mse(unsigned char origin[][256], unsigned char gen[][256]);
void gnuplot(double date[64][1024]);
void gnuplot2(int date[64], int Q);
void gnuplot2_2(int date[10][64]);
void my_copy(char* from[], char* to[]);
int err(const char* s);
void segmentation_ent_out(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], int mpans[][64], int block_flag[1024], int Q);
double cor_coe(double date1[1024], double date2[1024]);
void gnuplot3(double date1[1024], double date2[1024]);
void gnuplot4(double date1[7][1024]);
void gnuplot5(double date1[64], int name);
int img_out(unsigned char origin[][256], int date1[1024], int name);
int txt_out(double date1[4][1024], static char filename[20], int rate);
int txt_out2(double date1[65][1024], static char filename[20], int rate);
int txt_out3(double date1[64], static char filename[20], int rate);
int txt_out4(double date1[64][64], static char filename[20], int rate);
int group(double date1[65][1024], static char filename[20], int rate);
void ent_out(unsigned char origin[][256], double y[][1024], double avg[1024], double w[][64], double y_flag[64][1024], int block_flag[1024], int Q);
void ent_count(double y[256][256], double ica_dc[1024]);
void ent_count_basis(double w[64][64], double result_w[64]);
void img_out2(unsigned char dct[][256], unsigned char ica[][256], int block_flag[1024], int name);
void img_out3(unsigned char origin[][256]);
void img_out4(unsigned char origin[][256], int data1[1024], int data2[1024], int name);
double SSIM(unsigned char data1[][256], unsigned char data2[][256], int width, int heigth);
double b_SSIM(unsigned char data1[][8], unsigned char data2[][8], int width, int heigth);
//double sort(double date1[64][1024], double date2[64][1024], double out[64][1024]);
//double improvement(unsigned char filename[20], );




