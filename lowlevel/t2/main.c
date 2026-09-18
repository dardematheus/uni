#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar strings
#include <time.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#define REINHARD 1 
#define ACES 2

typedef unsigned char byte;

// Um pixel RGB (24 bits)
typedef struct
{
    unsigned char r, g, b;
} RGBPixel;

// Um pixel no formato RGBE (float)
typedef struct
{
  float r, g, b, e;
} RGBEPixel;

// Uma imagem RGB
typedef struct
{
    int width, height;
    RGBPixel *pixels;
} ImgRGB;

// Uma imagem RGBE
typedef struct
{
    int width, height;
    RGBEPixel *pixels;
} ImgRGBE;

// Protótipos
void process(ImgRGBE* in, ImgRGB* out, float expose_factor, float desired_gamma, char *desired_mapping);
void carregaHeader(FILE* fp, ImgRGBE* img);
void carregaImagem(FILE* fp, ImgRGBE* img);
void expose(ImgRGBE* in, float factor);
void reinhard(ImgRGBE *in, float opt);
void aces(ImgRGBE *in, float opt);
void gamma_correction(ImgRGBE *in, float gamma);
void bits_conversion(ImgRGBE *in, ImgRGB *out);

float max_luminescence;

int 
main(int argc, char *argv[])
{
    // Inclua e processe os demais argumentos por linha de comando
    if (argc != 5)
    {
        printf("hdrvis [imagem .hdf]\n");
        exit(1);
    }

    float expose_factor = atof(argv[2]);
    float desired_gamma = atof(argv[3]);
    char *desired_mapping = argv[4];

    // Imagens de entrada e saída
    ImgRGBE entrada;
    ImgRGB  saida;

    //
    // PASSO 1: Leitura da imagem
    // A leitura do header deve ser feita na função carregaHeader
    FILE* arq = fopen(argv[1], "rb"); // abre em formato binário
    carregaHeader(arq, &entrada);

    // Exibe as dimensões na tela, para conferência
    printf("Entrada   : %s %d x %d\n", argv[1], entrada.width, entrada.height);

	saida.height = entrada.height;
	saida.width = entrada.width;
    // A leitura do restante da imagem deve ser feita na função carregaImagem
    carregaImagem(arq, &entrada);
	fclose(arq);

    // Cria imagem de saída e "zera" ela
    int tam = entrada.width * entrada.height;
    saida.pixels = (RGBPixel*)malloc(tam * sizeof(RGBPixel));
    memset(saida.pixels, 0, tam * sizeof(RGBPixel));

    printf("Processando...\n");
    // Aplica todo o processamento necessário (exposição, etc) na função process
    // (acrescente mais parâmetros conforme a necessidade)
    process(&entrada, &saida, expose_factor, desired_gamma, desired_mapping);

    // Grava a imagem de saída como JPEG para conferência (usando a função stbi_write_jpg, com qualidade 90)
    stbi_write_jpg("saida.jpg", saida.width, saida.height, 3, saida.pixels, 90);

    free(entrada.pixels);
    free(saida.pixels);
}

// Executa todo o pipeline de processamento, grava saída em out->pixels
void 
process(ImgRGBE* in, ImgRGB* out, float expose_factor, float desired_gamma, char *desired_mapping)
{
	const char *f1 = "reinhard";
	const char *f2 = "aces";

	void(*algorithms[4])(ImgRGBE* in, float desired) = {expose, reinhard, aces, gamma_correction};

	algorithms[0](in, expose_factor);

	if(strcmp(desired_mapping, f1) == 0) {
		algorithms[REINHARD](in, 0.0);
	}
	if(strcmp(desired_mapping, f2) == 0) {
		algorithms[ACES](in, 0.0);
	}

	algorithms[3](in, desired_gamma);
	bits_conversion(in, out);
}

void
expose(ImgRGBE* in, float factor)
{
	float luminescence;
	int pixels = in->height * in->width;
	float factor2 = pow(2.0f, factor);
	max_luminescence = 0.0f;

	for(int i = 0; i < pixels; i++) {

		in->pixels[i].r = factor2 * in->pixels[i].r;
		in->pixels[i].g = factor2 * in->pixels[i].g;
		in->pixels[i].b = factor2 * in->pixels[i].b;

		luminescence = (0.2126 * in->pixels[i].r) + (0.7152 * in->pixels[i].g) + (0.0722 * in->pixels[i].b);
		if(luminescence > max_luminescence) max_luminescence = luminescence;
	}
}

void
aces(ImgRGBE* in, float opt)
{
    int pixels = in->height * in->width;

    for(int i = 0; i < pixels; i++) {
        float r = in->pixels[i].r;
        float g = in->pixels[i].g;
        float b = in->pixels[i].b;

        float rx = (r * (2.51f * r + 0.03f)) / (r * (2.43f * r + 0.59f) + 0.14f);
        float gx = (g * (2.51f * g + 0.03f)) / (g * (2.43f * g + 0.59f) + 0.14f);
        float bx = (b * (2.51f * b + 0.03f)) / (b * (2.43f * b + 0.59f) + 0.14f);

        in->pixels[i].r = rx > 1.0f ? 1.0f : (rx < 0.0f ? 0.0f : rx);
        in->pixels[i].g = gx > 1.0f ? 1.0f : (gx < 0.0f ? 0.0f : gx);
        in->pixels[i].b = bx > 1.0f ? 1.0f : (bx < 0.0f ? 0.0f : bx);
    }
}

void
reinhard(ImgRGBE* in, float opt)
{
    int pixels = in->height * in->width;
    float vwhite2 = max_luminescence * max_luminescence;

    if (vwhite2 < 1e-6f) vwhite2 = 1e-6f;

    for(int i = 0; i < pixels; i++) {
        float r = in->pixels[i].r;
        float g = in->pixels[i].g;
        float b = in->pixels[i].b;

        float rx = (r * (1.0f + r / vwhite2)) / (1.0f + r);
        float gx = (g * (1.0f + g / vwhite2)) / (1.0f + g);
        float bx = (b * (1.0f + b / vwhite2)) / (1.0f + b);

        in->pixels[i].r = rx > 1.0f ? 1.0f : (rx < 0.0f ? 0.0f : rx);
        in->pixels[i].g = gx > 1.0f ? 1.0f : (gx < 0.0f ? 0.0f : gx);
        in->pixels[i].b = bx > 1.0f ? 1.0f : (bx < 0.0f ? 0.0f : bx);
    }
}

void
gamma_correction(ImgRGBE *in, float gamma)
{
	int pixels = in->height * in->width;
	if(gamma <= 0.0) gamma = 1.0f;
	float div = 1.0f / gamma;
	
	for(int i = 0; i < pixels; i++) {
		in->pixels[i].r = pow(in->pixels[i].r, div);
		in->pixels[i].g = pow(in->pixels[i].g, div);
		in->pixels[i].b = pow(in->pixels[i].b, div);
	}

}

void
bits_conversion(ImgRGBE *in, ImgRGB *out)
{
	int pixels = in->height * in->width;

    for(int i = 0; i < pixels; i++) {
        float r = in->pixels[i].r;
        float g = in->pixels[i].g;
        float b = in->pixels[i].b;

        if (r > 1.0f) r = 1.0f; else if (r < 0.0f) r = 0.0f;
        if (g > 1.0f) g = 1.0f; else if (g < 0.0f) g = 0.0f;
        if (b > 1.0f) b = 1.0f; else if (b < 0.0f) b = 0.0f;

        out->pixels[i].r = (unsigned char)(255.0f * r);
        out->pixels[i].g = (unsigned char)(255.0f * g);
        out->pixels[i].b = (unsigned char)(255.0f * b);
    }
}

// Esta função deverá ser utilizada para apenas ler o conteúdo do header
// e extrair a largura e altura da imagem
void 
carregaHeader(FILE* fp, ImgRGBE* img)
{
	char filetype[3];
	fread(filetype, sizeof(byte), 3, fp);
	fread(&img->width, sizeof(byte), 4, fp);
	fread(&img->height, sizeof(byte), 4, fp);
}

// Esta função deverá ser utilizada para carregar o restante
// da imagem (após ler o header e extrair a largura e altura corretamente)
void 
carregaImagem(FILE* fp, ImgRGBE* img)
{
	byte buffer[4];
	float c;
	int pixels = img->height * img->width;

	img->pixels = (RGBEPixel*)malloc(pixels * sizeof(RGBEPixel));
	if(img->pixels == NULL) {
		printf("ERROR: Failed to allocate memory for image.\nImage might be too big?\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < pixels; i++) {
		fread(buffer, sizeof(byte), 4, fp);
		c = pow(2, buffer[3] - 136);

		img->pixels[i].r = buffer[0] * c;
		img->pixels[i].g = buffer[1] * c;
		img->pixels[i].b = buffer[2] * c;
		img->pixels[i].e = buffer[3];
	}
}

