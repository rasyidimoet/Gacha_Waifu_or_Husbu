#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_PANJANG_NAMA 1000
#define MAX_CEWE 200
#define MAX_BINI 10

typedef struct {
    char name[MAX_PANJANG_NAMA];
    int affection;
} Cewe;

typedef enum {
    MODE_WAIFU,
    MODE_HUSBU
} Game_mode;

typedef struct {
    char name[MAX_PANJANG_NAMA];
    int relationship;
    char status_cewe[20];
} Bini;

typedef struct {
    char name[MAX_PANJANG_NAMA];
    Bini bini_list[MAX_BINI];
    int bini_count;
} User;

void display_menu();
void how_to_play();
void start_game();
void get_random_cewe(char *nama_cewe);
int gacha_relationship(int option, const char *nama_cewe);
void tambahkan_bini(User *user, const char *nama_cewe, int relationship, const char *status_skrg);
void tampilkan_list_bini(const User *user);
int cari_lagi(User *user);
void process_game_logic(User *user);
int chance_gacha(int option);
void display_daftar_cewe(int index);
void clear_input_buffer();
void get_user_name(User *user);
void clean_string(char *str);
void safe_string_copy(char *dest, const char *src, int max_len);
int cewe_sudah_ada(const User *user, const char *nama_cewe);
void ganti_mode();

char *daftar_nama_cewe[] = {
    "Zero Two", "Hatsune Miku", "Rem", "Rias Gremory", "Megumin", "Saber", "Mai Sakurajima", 
    "Nami", "Power", "Albedo", "Asuna", "Bahlil Lahadalia", "Makima", "Mikasa Ackerman", "Nezuko Kamado", "Nico Robin", 
    "Miku Nakano", "Agnes Tachyon", "Emilia", "Violet Evergarden", "Hange Zoe", "Shinobu Kochou", "Aqua", "Akame", 
    "Marin Kitagawa", "Ram", "Nino Nakano", "Rin Tohsaka", "Yumeko Jabami", "Esdeath", "Himiko Toga", "Akeno Himejima", 
    "Kaguya Shinomiya", "Yor Forger", "Frieren", "Chika Fujiwara", "Kurumi Tokisaki", "Rei Ayanami", "Asuka Langley Souryuu", 
    "Nobara Kugisaki", "Hinata Hyuuga", "Erza Scarlet", "Mitsuri Kanroji", "Ayunda Risu", "Komi Shouko", "Yamato", "Ai Hoshino", 
    "Maki Zenin", "Gawr Gura", "Darkness", "Boa Hancock", "Shiro", "Ai Hayasaka", "Reze", "Ahmad Sahroni", "Tohru", "Sinon", "Neferpitou", 
    "Kurisu Makise", "Raphtalia", "Ochako Uraraka", "Truck-kun", "Rikka Takanashi", "Jinx", "Chiaki Nanami", "Nyan Cat", 
    "Itsuki Nakano", "Tatsumaki", "Misa Amane", "Lucy Heartfilia", "Yotsuba Nakano", "Ryuuko Matoi", "Kanna Kamui", "Mirko", 
    "Yoruichi Shihoin", "Rukia Kuchiki", "Jolyne Cujoh", "Venom", "Lucy", "Nagatorosan", "Crona", "Fubuki", "Cheval Grand", 
    "Oguri Cap", "Special week", "Cyrene", "Castorice", "Biwa Hayahide", "Vestia Zeta", "Kobo Kanaeru",
};

int total_cewe = sizeof(daftar_nama_cewe) / sizeof (daftar_nama_cewe[0]);

char *daftar_nama_cowo[] = {
    "Satoru Gojo", "Kirby", "Monkey D. Luffy", "Roronoa Zoro", "Levi", "Yamato", "Bongo Cat", 
    "Sukuna", "Yuji Itadori", "Neferpitou", "Eren Jaeger", "Truck-kun", "Nyan Cat", "Sanji", 
    "Killua Zoldyck", "Venom", "Joker", "Son Goku", "Denji", "Naruto Uzumaki", "Megumi Fushiguro", 
    "Crona", "Toji Fushiguro", "Ken Kaneki", "L", "Katsuki Bakugou", "Kento Nanami", "Osamu Dazai", 
    "Leon S. Kennedy", "Suguru Geto", "Tanjirou Kamado", "BMO", "Dio Brando", "Guts"
};

int total_cowo = sizeof(daftar_nama_cowo) / sizeof (daftar_nama_cowo[0]);

Game_mode current_mode = MODE_WAIFU;

void (*menu_functions[])() = {start_game, how_to_play, exit};

int cewe_sudah_ada(const User *user, const char *nama_cewe) {
    for (int i = 0; i < user->bini_count; i++) {
        if (strcmp(user->bini_list[i].name, nama_cewe) == 0) {
            return 1; 
        }
    }
    return 0;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validate_input_recursive(int min, int max){

    int choice;
    char input[10];
    
    printf("Masukkan pilihan anda (%d-%d): ", min, max);
    scanf("%s", input);

    int valid = 1;
    for(int i = 0; input[i] != '\0'; i++) {
        if(!isdigit(input[i])) {
            valid = 0;
            break;
        }
    }

    if(!valid) {
        printf("Input tidak valid! Silahkan masukkan angka! \n");
        return validate_input_recursive(min, max);
    }

    choice = atoi(input);

    if(choice < min || choice > max){
        printf("Pilihan tidak tersedia! Silahkan coba lagi. \n");
        return validate_input_recursive(min, max);
    }

    return choice;
}

void get_user_name(User *user){
    printf("Masukan nama paling greget yg lu punya: ");
    clear_input_buffer();
    
   
    fgets(user->name, MAX_PANJANG_NAMA, stdin);

    size_t len = strlen(user->name);
    if (len > 0 && user->name[len-1] == '\n') {
        user->name[len-1] = '\0';
    }
    
   
    if (strlen(user->name) == 0) {
        strcpy(user->name, "Karbit");
    }
}

void display_menu() {
    printf("---------- SIMULATOR NGEKARBIT ---------\n");
    printf("Seberapa karbit kamu dalam permainan ini. \n");
    printf("Pilihan menu: \n");
    printf("1. Langsung saja gaskan\n");
    printf("2. Cara mainnya gimana woy \n");
    printf("3. Pilih mode (Waifu / Husbu)\n");
    printf("4. Dahlah males, besok ajah \n");
    printf("-----------------------------------------\n");
}

void how_to_play() {
    printf("------------------------------- CARA MAEN ------------------------------------------\n");
    printf("1. Masukin nama, gelar, julukan, daerah asa-(serah lu dh). Misal nih (Raja Karbeat) \n");
    printf("2. Nanti bakal ada tuh cewe ato cowo yang datang ke elu \n");
    printf("3. Habis tuh lu mau ngapain dah ama dia, ada 4 pilihan nih kayak: \n");
    printf("   1. Jadiin bini ato husbu gw lah! \n");
    printf("   2. Jadiin pacar gw cocok nih! \n");
    printf("   3. Temen aja lah, boleh bgt nih \n");
    printf("   4. Skip ga minat \n");
    printf("4. Misal berhasil, cewe ato cowo itu masuk ke daftar bini ato husbu luwh \n");
    printf("5. Lu bisa ngarbit lagi nih misal mo nambah bini ato husbu \n");
    printf("------------------------------------------------------------------------------------\n");

    printf("Tekan enter untuk kembali ke main menu...");
    clear_input_buffer();
    getchar();
}

void ganti_mode() {
    printf("\n------------ CHOOSE YOUR TARGET! --------------\n");
    printf("Mau ngarbit yang mana nih?\n");
    printf("1. Nyari bini lah\n");
    printf("2. Udah pasti dong! My husbu!\n");
    printf("-----------------------------------\n");
    
    int choice = validate_input_recursive(1, 2);
    
    if (choice == 1) {
        current_mode = MODE_WAIFU;
        printf("\nBaiklh, kamu mo nyari bini mu! Persiapkan hokimu king!\n \n");
    } else {
        current_mode = MODE_HUSBU;
        printf("\nOkey, everything is ready! Semoga beruntung dapat husbumu!\n \n");
    }
}

void get_random_cewe(char *nama_cewe) {
    int random_index = rand() % total_cewe;

    if(current_mode == MODE_WAIFU) {
        random_index = rand() % total_cewe;
        strncpy(nama_cewe, daftar_nama_cewe[random_index], MAX_PANJANG_NAMA- 1);
    } else {
        random_index = rand() % total_cowo;
        strncpy(nama_cewe, daftar_nama_cowo[random_index], MAX_PANJANG_NAMA- 1);
    }
    
    nama_cewe[MAX_PANJANG_NAMA - 1] = '\0';
}

int chance_gacha(int option) {
    int base_chance;
    switch(option) {
        case 1: base_chance = 15; break;
        case 2: base_chance = 40; break;
        case 3: base_chance = 70; break;
        default: base_chance = 0;
    }

    int random_factor = rand() % 10;
    return base_chance + random_factor;
}

int gacha_relationship(int option, const char *nama_cewe) {
    int chance = chance_gacha(option);
    int roll = rand() % 100 + 1;

    printf("\nHubungan kamu ama %s \n", nama_cewe);
    printf("Kesempatan sukses: %d%%\n", chance);
    printf("Hasilnya: ");

    if(roll <= chance) {
        printf("SUKSES COY! \n");
        switch(option) {
            case 1: printf("\n%s jadi %s sekarang cihuyy \n", 
                    nama_cewe,
                    current_mode == MODE_WAIFU ? "Bini" : "Husbu",
                    current_mode == MODE_WAIFU ? "Binimu" : "Husbumu"); 
                break;
            case 2: printf("\n%s jadi pacarmu sekarang ea \n", nama_cewe); break;
            case 3: printf("\n%s jadi temenmu sekarang \n", nama_cewe); break;
            }
            return 1;
    } else {
        printf("AMPAS BGT ");
        switch(option) {
            case 1: printf("AWOKAOWKAKWOAKWK %s, gamau jadi %s luwh \n", 
                    nama_cewe,
                    current_mode == MODE_WAIFU ? "bini" : "Husbu"); 
                break;
            case 2: printf("Yahahahaha ditolak ama %s \n", nama_cewe); break;
            case 3: printf("%s gamau temenan ama lu, nt yh", nama_cewe); break;
            }
            return 0;
        }

}

void tambahkan_bini(User *user, const char *nama_cewe, int relationship, const char *status_skrg) {
    if (user->bini_count >= MAX_BINI){
        printf("\nUdah woy karbit amat lu!\n");
        return;
    }

    Bini *bini_baru = &user->bini_list[user->bini_count];
    strncpy(bini_baru->name, nama_cewe, MAX_PANJANG_NAMA - 1);
    bini_baru->name[MAX_PANJANG_NAMA - 1] = '\0';
    bini_baru->relationship = relationship;
    strncpy(bini_baru->status_cewe, status_skrg, 19); 
    bini_baru->status_cewe[19] = '\0'; 
    

    user->bini_count++;
    printf("%s udah masuk jadi harem lu bit!", nama_cewe);
}

void tampilkan_list_bini(const User *user) {

    if(user->bini_count == 0) {
        printf("Blm ada siapapun nih, kamu jomblo yh \n");
        return;
    }

    printf("\n--------------- MY BINI -----------------\n");
    printf("Total bini: %d\n\n", user->bini_count);

    for(int i =0; i < user->bini_count; i++){
        const Bini *ptr = &user->bini_list[i];
        printf("%d. %s\n", i + 1, ptr->name);
        printf("Status: ");
        switch(ptr->relationship) {
            case 1: printf("%s gw nih\n", current_mode == MODE_WAIFU ? "Bini" : "Husbu"); break;
            case 2: printf("Pacar gw nih\n"); break;
            case 3: printf("Sekedar temen ajh\n"); break;
        }
    }
    printf("\n-----------------------------------------\n");
}

int cari_lagi(User *user) {
    if (user->bini_count >= MAX_BINI) {
        printf("\n\nUdah woy karbit amat lu!\n");
        return 0; 
    }
    char choice;
    printf("\nApakah lu mau nambah lagi bit? (y/n): ");
    scanf(" %c", &choice);

    while(tolower(choice) != 'y' && tolower(choice) != 'n'){
        printf("Input tidak valid! Masukkan 'y' atau 'n' : ");
        scanf(" %c", &choice);
    }

    if(tolower(choice) == 'y'){
        printf("Bentar yh, gw nyari dulu nih..\n");
        return 1;
    } else {
        printf("Oh udahan, okelah bit!\n");
        return 0;
    }
}

void process_game_logic(User *user){
    char cewe_sekarang[MAX_PANJANG_NAMA];
    int lanjut_main = 1;
    int max_attempts = 20; 

    do {
        int attempts = 0;
        do {
            get_random_cewe(cewe_sekarang);
            attempts++;
            if (attempts >= max_attempts) break;
        } while (cewe_sudah_ada(user, cewe_sekarang)); 
        printf("\nHasil ngerkarbit mu kali ini adalah: ");
        printf("\nTadaaa kamu ketemu ama %s \n", cewe_sekarang);
        printf("\nKamu mau jadiin dia siapa mu nihhh??\n");
        printf("1. Jadiin %s gw lah! \n", current_mode == MODE_WAIFU ? "Bini" : "Husbu");
        printf("2. Jadiin pacar gw cocok nih! \n");
        printf("3. Temen aja lah, boleh bgt nih \n");
        printf("4. Skip ga minat \n");

        int choice = validate_input_recursive(1, 4);

        if(choice == 4){
            printf("\nKamu ninggalin %s...\n", cewe_sekarang);
            printf("Habis nih mo ngapain kamu?\n");
            printf("1. Nyari my %s\n", current_mode == MODE_WAIFU ? "Bini" : "Husbu" );
            printf("2. Ga dulu, capek\n");

            int leave_choice = validate_input_recursive(1, 2);

            if(leave_choice == 1) {
                continue;
            } else {
                lanjut_main = 0;
                break;
            }
        }
        int success = gacha_relationship(choice, cewe_sekarang);
        
        if(success) {
            const char *status;
            switch(choice){
                case 1: status = "Bini"; break;
                case 2: status = "Pacar"; break;
                case 3: status = "Teman"; break;
                default: status ="Unknown";
            }
            tambahkan_bini(user, cewe_sekarang, choice, status);
        }

        if(!cari_lagi(user)){
            lanjut_main = 0;
        }
    } while(lanjut_main && user->bini_count < MAX_BINI);

    if(user->bini_count > MAX_BINI){
         printf("\nUdah woy karbit amat lu!\n");
    }
    tampilkan_list_bini(user);
}

void start_game(){
    printf("\n------------- START GAME --------------\n");

    User user;
    user.bini_count = 0;

    get_user_name(&user);

    printf("Halo %s %s, selamat ngekarbit kali ini! \n", 
        user.name,
        current_mode == MODE_WAIFU ? "king" : "queen");

    process_game_logic(&user);

    printf("Arigatou nee %s %s dah main mampir ke sini. Kpn yh punya kisah kek gini\n", 
        user.name,
        current_mode == MODE_WAIFU ? "king" : "queen");
    printf("Tekan enter untuk kembali ke menu utama...");
    getchar();
    getchar();
}

int main() {
    srand(time(NULL));

    int running = 1;

    while(running) {
        display_menu();

        int choice = validate_input_recursive(1, 4);

        switch(choice) {
            case 1:
                start_game();
                break;
            case 2:
                how_to_play();
                break;
            case 3:
                ganti_mode();
                break;
            case 4:
                printf("Thenkyuu udah mampir ke sini!");
                running = 0;
                break;

        }   
    }

    return 0;
} 