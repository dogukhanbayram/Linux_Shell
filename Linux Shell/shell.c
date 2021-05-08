#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 

#define LISTEBOYUT 100 
 
#define clear() printf("\033[H\033[J") 
  
//ilk önce ekranı temizler, sonra kullanıcı adını yazdırır ve yeniden ekranı temizler
void baslat() 
{ 
    clear(); 
    char* kullaniciAdi = getenv("USER"); 
    printf("\n\n\nKullanıcı Adı: @%s", kullaniciAdi); 
    printf("\n"); 
    sleep(1); 
    clear(); 
} 
  
//kullanıcıdan aldığı girişleri veya dosyadan okuduğu stringleri giriş stringine atar
int girisAl(char* str, int* dosyaModu,char* dy) 
{ 
    int i = 0;
    char buf[1000];
    fflush(stdin);
    printf("%s", "\n >>");
    if(*dosyaModu == 1)
    {
        fgets(buf,1000,stdin);
    }
    else if(*dosyaModu == 2)
    {
        FILE* fp;
        fp = fopen(dy, "r");
        if(fp == NULL) 
        {
            printf(" Batch dosyası mevcut değil veya açılamıyor\n");
            exit(0);
        }
        fgets(buf,1000,fp);
        fclose(fp);
        *dosyaModu=1;
    }
    
    strtok(buf, "\n");
    if (strlen(buf) != 0) { 
        strcpy(str, buf); 
        return 0; 
    } else { 
        return 1; 
    } 
} 
  
//bulunulan dizini yazdırır
void dizinYaz() 
{ 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\nDir: %s", cwd); 
} 
  
//yardim menüsünü açar
void yardimAc() 
{ 
    puts(
        "\nYARDIM"
        "\nKomut Listesi:"
        "\n>quit"
        "\n>help"
        "\n>hello"
    );
    return; 
} 
  
//giriş stringinde komut varsa onları çalıştırır yoksa da uygun bir uyarı verir
int komutCalistir(char** parsed) 
{ 
    int komutSayi = 3, i;
    char* komutListesi[komutSayi]; 
    char* kullaniciAdi; int b = 0;
    int g = 0;
    pid_t pid,wpid;int sts = 0;


    int mlc;
    char** komutDizisi = malloc(LISTEBOYUT * sizeof(char));
    for(mlc = 0;mlc < LISTEBOYUT;mlc++)
    {
    komutDizisi[mlc] = malloc(LISTEBOYUT * sizeof(char));
    }
    free(komutDizisi);
    komutDizisi = malloc(LISTEBOYUT * sizeof(char));
    for(mlc = 0;mlc < LISTEBOYUT;mlc++)
    {
    komutDizisi[mlc] = malloc(LISTEBOYUT * sizeof(char));
    }


    komutListesi[0] = "quit"; 
    komutListesi[1] = "help"; 
    komutListesi[2] = "hello";

    while(parsed[b])
    {
        if (strcmp(parsed[b], komutListesi[0]) == 0)
        { 
            printf("\nGule Gule\n");     
            exit(0);
        }
        b++;
    }
    b=0;g=0;int h = 5;int kmt;int kmth;int r; int rh;
    while (parsed[b])
    { 
        if((pid = fork()) == 0)
        {   
            if(strcmp(parsed[b], komutListesi[1]) == 0)
                { 
                yardimAc();
                exit(0);
                }
            else if(strcmp(parsed[b], komutListesi[2]) == 0)
                {                   
                kullaniciAdi = getenv("USER"); 
                printf("Merhaba %s.\n",kullaniciAdi);   
                   printf("Yardim icin help yazin.\n"); 
                    exit(0);                
                }
                else
                {
                    h = 0;kmt = 0;kmth = 0;
                    while(parsed[b][h] != '\0')
                    {
                        if(parsed[b][h] != ' ')
                        {
                            komutDizisi[kmt][kmth]=parsed[b][h];
                            kmth++;
                        }
                        else if(parsed[b][h+1] != ' ')
                        {
                            kmt++;
                            kmth = 0;
                        }       
                        h++;   
                    }
                    kmt++;
                    komutDizisi[kmt]=NULL;
                    if(execvp(komutDizisi[0],komutDizisi) < 0)
                    {
                        printf("%s : Hatalı Komut\n",komutDizisi[kmt - 1]);
                        exit(0);
                    }
                    execvp(komutDizisi[0],komutDizisi);
                    exit(0);
                }   
        }
        b++;
    }
    b=0;g=0;
    while ((wpid = wait(&sts)) > 0)
    {
        
    }
    free(komutDizisi);

    return 0; 
} 
//giriş stringinden noktalı virgülleri siler
int nvirgulSil(char* str, char** parsed)
{
    int i;int p = 0;
    for (i = 0; i < LISTEBOYUT; i++) 
    { 
        parsed[i] = strsep(&str, ";");
        p++;
        if (parsed[i] == NULL)
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 

    } 
    return p;
}

//giriş stringinden boşlukları siler
void boslukSil(char** parsed,int p) 
{ 
    int i;
    int j;
    char* temp;
    int l;int cont;

    for (i = 0; i < (p-1); i++) 
    { 
        cont = 0;
        while(parsed[i][0] == ' ')
        {
            j = 0;
            while(parsed[i][j] != '\0')
            {
                parsed[i][j] = parsed[i][j+1];
                j++;
            }
        }
        j = 0;
        while(parsed[i][j] != '\0')
        {
            if(parsed[i][j] == ' ')
            {
                l = j;
                while(parsed[i][l]!='\0')
                {
                    if(parsed[i][l] != ' ')
                    {
                        cont = 1;
                    }

                    l++;
                }
            }
            j++;
        }
        j--;
        while(parsed[i][j] == ' ')
        {
            parsed[i][j] = '\0';
            j--;
        }
        if(cont == 0)
        {
            parsed[i] = strsep(&parsed[i], " ");
        }
        if (parsed[i] == NULL)
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 

    } 
} 
//giriş stringindeki kural hatalarını bulur ve ona göre ekrana hatayı yazdırır
int Kontrol(char* str)
{
    int don = 1;
    int i;
    if(str[0] == ';')
    {
        don = 0;
        printf("İlk karakter noktalı virgül olamaz");
        return don;
    }
    if(strlen(str)>512)
    {
        don=0;
        printf("Komut satırı en fazla 512 karakter olabilir");
        return don;
    }
    for(i=0;i<LISTEBOYUT;i++)
    {
    if(str[i] == ';')
    {
        if(str[i+1] == ';')
        {
            printf("Peş peşe iki tane noktalı virgül olamaz");
            don = 0;
            return don;
        }
        else if(str[i+1] =='\0')
        {
            printf("Son karakter noktalı virgül olamaz");
            don = 0;
            return don;
        }
    }
    }
    return don;
}
//giris stringini komutCalistir'in algilayabileceği bir şekile getirip komutCalistir'da çalıştırır
int stringIsle(char* str, char** parsed) 
{ 
    if(Kontrol(str) == 0)
    {
        printf("%s\n"," << HATA, Başka komut girin");
    }
    else
    {
        int p = nvirgulSil(str, parsed);
        boslukSil(parsed,p);
        komutCalistir(parsed);
    }
    return 0;
} 
  
int main(int Argc, char* Argv[]) 
{ 
    char girisString[1000], *duzenliString[LISTEBOYUT];
    baslat(); 
    int z = 0;
    Argv[2] = NULL;
    int* DM;
    *DM=0;
    char *dosyaYolu;
    //CTRL+D veya quit komutu girilene kadar devamlı çalışır
    while (!feof(stdin)) { 

        int i;
        for(i = 0; i < 1000;i++)
        {
            girisString[i]='\0';
        } 
        for(i = 0; i < 100;i++)
        {
            duzenliString[i]='\0';
        } 
        
        dizinYaz(); 
        
        if(Argc == 2 && *DM == 0)
        {
            *DM = 2;
            dosyaYolu = Argv[1];
        }
        else
        {
            *DM=1;
        }
        if (girisAl(girisString,DM,dosyaYolu)) 
            continue; 
        
        stringIsle(girisString, duzenliString); 
        *DM=1;
        
    } 
    return 0; 
} 