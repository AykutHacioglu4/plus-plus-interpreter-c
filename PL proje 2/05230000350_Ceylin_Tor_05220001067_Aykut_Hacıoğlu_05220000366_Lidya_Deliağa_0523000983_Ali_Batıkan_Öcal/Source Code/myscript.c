#include <stdio.h> //printf, fgets işlemleri
#include <stdlib.h> //exit, atoll
#include <string.h> //strcpy, strcat gibi string işlemleri
#include <ctype.h> //isspace, isdigit, isalpha gibi karakter kontrolleri

#define MAX_VARS 256
#define MAX_LINE 1024 //satir max ch
#define MAX_NAME 21 //20 karakter + null
#define MAX_PROGRAM_LINES 1000 

// değişkenin bilgilerini tutan yapi
typedef struct {
    char name[MAX_NAME];
    char declared;
    char initialized;
    long long value;
} Variable;

// değişken listesi
Variable vars[MAX_VARS];
int var_count = 0;

// .ppp dosyasını bellekte tutan matris
char all_program_lines[MAX_PROGRAM_LINES][MAX_LINE];
int total_program_lines = 0;

// bosluklari siler
void trim(char *str) {
    int i = 0, j = 0;
    while (isspace((unsigned char)str[i])) i++; 
    while (str[i]) str[j++] = str[i++];         
    str[j--] = '\0';                           
    while (j >= 0 && isspace((unsigned char)str[j])) str[j--] = '\0'; 
}

// Satır boş mu veya yorum mu
int is_comment_or_empty(const char *line) {
    return (line[0] == '\0' || line[0] == '*');
}

//Değişken bulma
Variable* find_var(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return &vars[i];
    }
    return NULL; 
}

// degisken tanımlama
void declare_var(const char *name, int line_num) {
    // degisken var mi yok mu kontrol
    if (find_var(name)) {
        return;
    }
    // degisken limit kontrolu
    if (var_count >= MAX_VARS) {
        printf("Error on line %d: Too many variables (max %d).\n", line_num, MAX_VARS);
        exit(1);
    }
    //yeni degiskene baslangis degeri verme
    strncpy(vars[var_count].name, name, MAX_NAME - 1);
    vars[var_count].name[MAX_NAME - 1] = '\0';
    vars[var_count].value = 0;         // deger 0
    vars[var_count].declared = 1;
    vars[var_count].initialized = 1;   // başlangıç değeri verildi 1 oldu
    var_count++;
}

//değer atama
void set_var(const char *name, long long value, int line_num) {
    Variable *var = find_var(name);
    if (!var) {
        printf("Error on line %d: Undeclared variable '%s'.\n", line_num, name);
        exit(1);
    }
    var->value = value;
    var->initialized = 1;
}

// değer okuma
long long get_var(const char *name, int line_num) {
    Variable *var = find_var(name);
    if (!var) {
        printf("Error on line %d: Undeclared variable '%s'.\n", line_num, name);
        exit(1);
    }
    if (!var->initialized) {
        printf("Error on line %d: Uninitialized variable '%s'.\n", line_num, name);
        exit(1);
    }
    return var->value;
}

// degisken adinin gecerliligi kontrol ediliyor
int is_valid_name(const char *name) {
    if (!name || *name == '\0') return 0; // ad bos olamaz
    if (!isalpha((unsigned char)name[0])) return 0; // harf ile baslamali
    for (int i = 1; name[i]; i++) {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_') return 0; // harf sayi altcizgi olabilir
    }
    return 1;
}

// gecerli sayi olup olmadigi kontrol edilir
int is_valid_number(const char *val) {
    if (!val || *val == '\0') return 0; // bos olamaz
    int i = 0;
    if (val[0] == '-') { // onde gelen - izin verilir
        i++;
        if (val[1] == '\0') return 0; // "-" tek basina gecerli sayi degil
    }
    // sonraki karakter sayi olmali
    for (; val[i]; i++) {
        if (!isdigit((unsigned char)val[i])) return 0;
    }
    return 1;
}

// fonksiyon henuz tanimlanmadan once bildiriliyor
void execute_program_block(int start_idx, int end_idx, int base_line_num);

// tek satirlik kodlari yorumlar repeat haric
void interpret_single_line(const char *line, int line_num) {
    // noktali virgul kontrolu
    if ((strncmp(line, "number", 6) == 0 ||
         strstr(line, ":=") != NULL ||
         strstr(line, "+=") != NULL ||
         strstr(line, "-=") != NULL ||
         strncmp(line, "write", 5) == 0) && strchr(line, ';') == NULL) {
        printf("Syntax error on line %d: Missing ';' at end of statement.\n", line_num);
        exit(1);
    }


    // degisken tanimlama
    if (strncmp(line, "number", 6) == 0) {
        char name[MAX_NAME];
        if (sscanf(line + 6, " %20[^;];", name) == 1 && is_valid_name(name)) { 
            trim(name);
            declare_var(name, line_num);//tanimlama
            // sscanf başarısızsa ya da isim geçerli değilse
        } else {
            printf("Syntax error on line %d: Invalid 'number' declaration.\n", line_num);
            exit(1);
        }


        // atama islemi
    } else if (strstr(line, ":=") != NULL) {
        char var[MAX_NAME], val_str[MAX_LINE]; //solda değişkenin ismi sagda atanan deger
        if (sscanf(line, "%20[^:=]:=%255[^;];", var, val_str) == 2) { // satırı parçalar
            // := operatöründen önceki kısmı var değişkenine atar 
            // ; işaretine kadar olan kısmı val_str'a atar
            trim(var); trim(val_str); //bosluklari temizler
            if (!is_valid_name(var)) {
                printf("Error on line %d: Invalid variable name '%s' in assignment.\n", line_num, var);
                exit(1);
            }
            if (is_valid_number(val_str)) { //atanacak deger sayi ise
                set_var(var, atoll(val_str), line_num); // atoll ile long long değere çevrilir set_var ile atanir
            } else if (is_valid_name(val_str)) {
                set_var(var, get_var(val_str, line_num), line_num);//değişkense get_var ile değeri alınır ve yine set_var ile atanır
            } else {
                printf("Error on line %d: Invalid value '%s'. Must be a number or declared variable.\n", line_num, val_str);
                exit(1);
            }
           
        } else {
            printf("Syntax error on line %d: Invalid assignment statement.\n", line_num);
            exit(1);
        }


        //arttirma islemi
    } else if (strstr(line, "+=") != NULL) {
        char var[MAX_NAME], val_str[MAX_LINE];
        if (sscanf(line, "%20[^+=]+= %255[^;];", var, val_str) == 2) {// satırı parçalar

            trim(var); trim(val_str); //bosluklari temizler
            long long current_val = get_var(var, line_num); //var değişkeninin mevcut değeri alınır.
            if (is_valid_number(val_str)) { //atanacak deger sayiysa
                set_var(var, current_val + atoll(val_str), line_num); 
            } else {//Eğer sağ taraf sayı değilse degiskenin get_var ile degeri alinir 
                set_var(var, current_val + get_var(val_str, line_num), line_num);
            }
            // sscanf(...) başarısız olursa 
        } else {
            printf("Syntax error on line %d: Invalid '+=' assignment.\n", line_num);
            exit(1);
        }



        // azaltma islemi
    } else if (strstr(line, "-=") != NULL) {
        char var[MAX_NAME], val_str[MAX_LINE];

        if (sscanf(line, "%20[^-=]-= %255[^;];", var, val_str) == 2) {
            trim(var); trim(val_str);
            long long current_val = get_var(var, line_num);
            if (is_valid_number(val_str)) {
                set_var(var, current_val - atoll(val_str), line_num);
            } else {
                set_var(var, current_val - get_var(val_str, line_num), line_num);
            }
        } else {
            printf("Syntax error on line %d: Invalid '-=' assignment.\n", line_num);
            exit(1);
        }


        // satir write ile baslayip baslamadiginin kontrolu
    } else if (strncmp(line, "write ", 6) == 0) {
        char *p = (char *)line + 6; //  geri kalan içeriği işlemek için p pointeri olusturduk
        char token_buf[MAX_LINE]; // gecici buffer degisken ismi burda tutulur
        int in_string = 0; // string kontrolu icin flag

        while (*p) { // ch ch okunur
            while (isspace((unsigned char)*p)) p++; // bosluklari atlamak icin

            if (*p == '"') { // string baslangici
                p++; 
                while (*p && *p != '"') {
                    putchar(*p); // string icindekileri ekrana yazdirir
                    p++;
                }
                if (*p == '"') p++; // " bu varsa bunu gec
            } else if (strncmp(p, "newline", 7) == 0 && (isspace((unsigned char)p[7]) || p[7] == ';' || p[7] == '\0')) {
                //newline komutuysa ekrana yeni satir yazdirilir p ileri alinir >> 7
                printf("\n"); 
                p += 7;
                
            } else if (strncmp(p, "and", 3) == 0 && isspace((unsigned char)p[3])) {
                //andden sonra bosluk varsa baglactir bisi yapilmaz    and gecilir
                p += 3; 
            } else if (isalpha((unsigned char)*p)) { // harfle baslamasi gerekiyor
                int i = 0;
                while (isalnum((unsigned char)*p) || *p == '_') { // harf sayi _  ile devam edebilir
                    token_buf[i++] = *p;
                    p++;
                }
                token_buf[i] = '\0';
                // variable gecersizse error yazdirilir
                if (!is_valid_name(token_buf)) {
                    printf("Error on line %d: Invalid variable name '%s' in write statement.\n", line_num, token_buf);
                    exit(1);
                }

                printf("%lld", get_var(token_buf, line_num)); // Write variable value
            
            } else {
                // beklenmeyen karakter ise atlanir
                p++;
            }
        }
        printf("\n"); // her write komutu sonrasi yeni satir eklenir
        // operator string write kontrolu vs degilse error
    } else {
        printf("Syntax error on line %d: Unrecognized statement '%s'.\n", line_num, line);
        exit(1);
    }
}




//  all_program_lines dizisindeki start_idx ile end_idx aralığındaki satırları işler
//  base_line_num gerçek satır numarası (hata mesajları doğru gözüksün diye)

void execute_program_block(int start_idx, int end_idx, int base_line_num) {
    int current_relative_line = 0; //bloğun içinde kaç satır ilerlediğimizi sayar counter ve hata mesajlarında satir sayisi

    for (int i = start_idx; i < end_idx; i++) {
        char line_copy[MAX_LINE];
        strcpy(line_copy, all_program_lines[i]);
        trim(line_copy);

        int current_actual_line_num = base_line_num + current_relative_line; // bu satırın dosya üzerindeki sıra numarasını hesaplar

        if (is_comment_or_empty(line_copy)) {//satir bos veye yorumsa atla
            current_relative_line++;
            continue;
        }

        //repeat varsa
        if (strncmp(line_copy, "repeat ", 7) == 0) { 
            char counter_str[MAX_NAME]; //tekrar sayısını geçici olarak saklamak için
            long long repeat_value; 
            char *ptr = line_copy + 7; //repeati atlat
            while (*ptr == ' ') ptr++; // bosluklari atla
            sscanf(ptr, "%20[^ ]", counter_str); // repeatten sonra geleni counter_str icine alir
            trim(counter_str);

            if (is_valid_number(counter_str)) {
                repeat_value = atoll(counter_str); //counter_str deki değeri sayıya çevirerek bu değişkene atariz
            } else {
                repeat_value = get_var(counter_str, current_actual_line_num); //değişken adıysa o değişkenin değerini alır
            }

            //{} aralığını tanımlamak
            int block_start_idx = i + 1; 
            int block_end_idx = -1;
            int brace_level = 0;

            // { aynı satırdaysa 
            if (strchr(line_copy, '{') != NULL) {
                brace_level = 1;   // {  gordugu icin 1 oldu ilerde {} esitligi icin kullanilacak
                block_start_idx = i + 1; // block icerigi sonraki satirda baslatiliyor
            } else {
                // sonraki satirlarda { araniyor 
                int j = i + 1;
                while (j < end_idx && j < total_program_lines) { // kodun sınırlarını aşmamak için 
                    char temp_line[MAX_LINE];
                    strcpy(temp_line, all_program_lines[j]);
                    trim(temp_line);
                    current_relative_line++; //bu satir islendi bir artti
                    if (strcmp(temp_line, "{") == 0) {  //trimlenen satirda { kaldiysa blok baslar
                        brace_level++;
                        block_start_idx = j + 1; // block icerigi sonraki satirda baslatiliyor
                        break;
                    }
                    j++;
                }
                // { hiç bulunamazsa hata verilir.
                if (brace_level == 0) {
                    printf("Syntax error on line %d: Missing opening '{' for repeat block.\n", current_actual_line_num);
                    exit(1);
                }
            }

            // {} dengesi
            int j = block_start_idx;  
            while (j < end_idx && j < total_program_lines) {// kodun sınırlarını aşmamak için 
                char temp_line[MAX_LINE];
                strcpy(temp_line, all_program_lines[j]);
                trim(temp_line);
                current_relative_line++; //bu satir islendi bir artti

                if (strcmp(temp_line, "{") == 0) { //trimlenen satirda { kaldiysa blok baslar
                    brace_level++; // Nested  {
                } else if (strcmp(temp_line, "}") == 0) {
                    brace_level--; // esitlik icin azaltilir
                    if (brace_level == 0) {
                        block_end_idx = j; //bitis blogu } indexi
                        break;
                    }
                }
                j++;
            }
            //parantez dengesi bozulmuşsa veya hic } bulunmamissa error
            if (brace_level != 0 || block_end_idx == -1) {
                printf("Syntax error on line %d: Missing closing '}' for repeat block.\n", current_actual_line_num);
                exit(1);
            }

            // döngünün kaç defa tekrar edeceği 
            for (long long k = repeat_value; k > 0; k--) {
                // Sayaç bir değişken adıysa recursive için değeri güncellenir
                if (!is_valid_number(counter_str)) {
                    set_var(counter_str, k, current_actual_line_num);
                }
                execute_program_block(block_start_idx, block_end_idx, current_actual_line_num + 1);//belirlenen blok içerisindeki satırlar arasinda
            }
            // degiskense döngü bittikten sonra değeri sıfırlanır
            if (!is_valid_number(counter_str)) {
                set_var(counter_str, 0, current_actual_line_num);
            }

            // i   `}` satırına kadar atlatılır
            i = block_end_idx;
        } else {
            // satir repeat bloku degilse interpret_single_line(...) çağrılır
            interpret_single_line(line_copy, current_actual_line_num);
        }
        current_relative_line++; // satır sayısını izlemek için sayac artar
    }
}
//program calısmaya burada baslar
int main(int argc, char *argv[]) { //komut satırından girilen argüman bilgileri
    char filename[256];
    if (argc == 2) { // örneğin ./ppp myscript gibi çağırınca .ppp yi kendi ekler
        snprintf(filename, sizeof(filename), "%s.ppp", argv[1]);
    } else { // komut satırına dosya ismi girilmezse
        printf("Enter filename (without .ppp): ");
        if (scanf("%255s", filename) != 1) { // girdi alınır
            fprintf(stderr, "Error reading filename.\n");
            return 1;
        }
        strcat(filename, ".ppp"); //.ppp uzantısı eklenir
    }

    FILE *file = fopen(filename, "r");
    if (!file) { //eğer dosya açılamazsa error verir ve çıkar
        perror("Error opening file");
        return 1;
    }

    // Read the entire file into memory
    int current_line_in_file = 0;
    //her satır  diziye aktarılır ve her bir satır bittikten sonra sonuna \0 ekler
    while (fgets(all_program_lines[current_line_in_file], sizeof(all_program_lines[0]), file)) {  
        trim(all_program_lines[current_line_in_file]); //her satır okunduğu gibi boşluklar temizlenir
        current_line_in_file++; //satır sayacı
        if (current_line_in_file >= MAX_PROGRAM_LINES) { //satır sayısı sınırı(1000) geçerse error verir
            fprintf(stderr, "Error: Program exceeds maximum line limit of %d.\n", MAX_PROGRAM_LINES);
            fclose(file);
            return 1;
        }
    }
    total_program_lines = current_line_in_file; //toplam satır sayısı atanması
    fclose(file); //dosya kapatılır

    //tüm satırlar belleğe yüklenince çağırılır
    execute_program_block(0, total_program_lines, 1); 

    return 0;
}