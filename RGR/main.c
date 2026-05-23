#include <curl/curl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <cjson/cJSON.h>

struct Memory{
    char *data;
    size_t size;
}typedef Memory;

struct ThreadUser{
    int id;
    time_t time_start;
    int time_work;
    int count;
    cJSON* host;
    cJSON **req_arr;
    cJSON *network;
    struct RequestMetrics *req_metrix;
    FILE *file_log;
    pthread_mutex_t *mutex_log;
    int log_mode;
};
struct RequestMetrics{
    long *http_code;           
    double *total_time;        
    double *ttfb;              
    double *dns_time;          
    double *connect_time;      
    curl_off_t *size_down;
    int error_curl;
    int error_http;
    int success_http;
    int max_req;    
};

int Сomparison(const void *a, const void *b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    size_t bytes = size * nmemb;
    Memory *memory = (Memory*)userdata;

    size_t new_size = memory->size + bytes + 1;

    char *data = realloc(memory->data, new_size);
    if (data == NULL){
        return 0;
    }

    memory->data = data;

    memcpy(memory->data + memory->size, ptr, bytes);
    memory->size = memory->size + bytes;
    memory->data[memory->size] = '\0';
    return bytes;
}

void Logger(FILE *file, char *text, int status){
    if (file == NULL){
        printf("Ошибка открытия Файла!\n");
        return;
    }

    switch (status)
    {
    case 0:
        fprintf(file, "[ERROR]: %s\n", text);
        printf("[ERROR]: %s\n", text);
        break;
    case 1:
        fprintf(file, "[WARNING]: %s\n", text);
        printf("[WARNING]: %s\n", text);
        break;
    case 2:
        fprintf(file, "[INFO]: %s\n", text);
        printf("[INFO]: %s\n", text);
        break;
    }
}

void *ThreadCurl(void *arg){
    Memory responce = {NULL, 0};
    struct ThreadUser *user = (struct ThreadUser*)arg;
    int log_mod = user->log_mode;
    struct curl_slist *headers_arr[user->count]; 
    for (int i = 0; i < user->count; i++) {
        headers_arr[i] = NULL;
    }
    char *body_arr[user->count];
    for (int i = 0; i < user->count; i++) {
        body_arr[i] = NULL;
    }
    int delay_ms_arr[user->count];
    int auth_arr[user->count];
    unsigned int seed = time(NULL) ^ pthread_self();
    int weight_sum = 0;
    int weight_arr[user->count];

    
    CURL *handles[user->count]; 
    for (int i = 0; i < user->count; i++){
        handles[i] = curl_easy_init();
        if (handles[i] == NULL){
        printf("Ошибка создания пользователя!\n");
        free(responce.data);
        return NULL;
        }
    }
    for (int i = 0; i < user->count; i++){
        curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, &responce);

        cJSON *curl = user->req_arr[i];
        cJSON *auth_required = cJSON_GetObjectItem(curl, "auth_required");
        auth_arr[i] = auth_required->valueint;

        cJSON *behavior = cJSON_GetObjectItem(curl, "behavior");
        cJSON *weight = cJSON_GetObjectItem(behavior, "weight");
        if (weight != NULL) {
            weight_sum += weight->valueint;
            weight_arr[i] = weight->valueint;
        }
        cJSON *delay_ms = cJSON_GetObjectItem(behavior, "delay_ms");
        if (delay_ms != NULL){
            delay_ms_arr[i] = delay_ms->valueint;
        }
        else {
            delay_ms_arr[i] = 0;
        }

        cJSON *path = cJSON_GetObjectItem(curl, "path");
        cJSON *query = cJSON_GetObjectItem(curl, "query");
        char *path_str = path->valuestring;
        char *host = user->host->valuestring;
        char url[512] ="";
        if(host[strlen(host)-1] == '/'){
            host[strlen(host)-1] = '\0';
        }
        snprintf(url + strlen(url), sizeof(url) - strlen(url), "%s%s", host, path_str);
        cJSON *query_arg = NULL;
        if (query != NULL){
            query_arg = query->child;
        }
        while(query_arg != NULL){
            char *arg_str = query_arg->string;
            char arg_value[256];
            if (url[strlen(url)-1] == '/'){
                url[strlen(url)-1] = '?';
            }
            if (strcmp(query_arg->valuestring, "str") == 0){
                snprintf(arg_value, sizeof(arg_value), "user%d", user->id);  
            }
            else{
                snprintf(arg_value, sizeof(arg_value), "%s", query_arg->valuestring);
            }
            snprintf(url + strlen(url), sizeof(url) - strlen(url), "%s=%s&", arg_str, arg_value);
            query_arg = query_arg->next;
        }
        if (url[strlen(url)-1] == '&'){
            url[strlen(url)-1] = '\0';
        }
        curl_easy_setopt(handles[i], CURLOPT_URL, url);

        cJSON *metod = cJSON_GetObjectItem(curl, "method");
        char *metod_str = metod->valuestring;
        curl_easy_setopt(handles[i], CURLOPT_CUSTOMREQUEST, metod_str);


        cJSON *headers_json = cJSON_GetObjectItem(curl, "headers");
        cJSON *header = NULL;
        if (headers_json != NULL) {
            header = headers_json->child;
        }
        while(header != NULL){
            char *header_arg = header->string;
            char *header_value = header->valuestring;
            char header_str[512] = "";
            snprintf(header_str, sizeof(header_str), "%s: %s", header_arg, header_value);
            headers_arr[i] = curl_slist_append(headers_arr[i], header_str);
            header = header->next;
        }
        curl_easy_setopt(handles[i], CURLOPT_HTTPHEADER, headers_arr[i]);

        cJSON *body = cJSON_GetObjectItem(curl, "body");
        char body_s[1024] = "{"; 
        cJSON *body_key = NULL;
        if (body != NULL && body->child != NULL) {
            body_key = body->child;
            
            while(body_key != NULL){
                char *body_key_str = body_key->string;
                char body_value_str[64] = "";
                int body_value_int;
                if (strcmp(body_key->valuestring, "str") == 0){
                    snprintf(body_value_str + strlen(body_value_str), sizeof(body_value_str) - strlen(body_value_str), "user%d", user->id);
                    snprintf(body_s + strlen(body_s), sizeof(body_s) - strlen(body_s), "\"%s\":\"%s\",", body_key_str, body_value_str);
                }
                if (strcmp(body_key->valuestring, "int") == 0){
                    body_value_int = user->id;
                    snprintf(body_s + strlen(body_s), sizeof(body_s) - strlen(body_s), "\"%s\":%d,", body_key_str, body_value_int);
                }
                body_key_str = body_key->valuestring;
                body_value_int = body_key->valueint;
                if ((body_key_str != NULL) && !(strcmp(body_key->valuestring, "str") == 0)){
                    snprintf(body_s + strlen(body_s), sizeof(body_s) - strlen(body_s), "\"%s\":\"%s\",", body_key_str, body_value_str);
                }
                else if (!(strcmp(body_key->valuestring, "int") == 0) && !(strcmp(body_key->valuestring, "str") == 0)){
                    snprintf(body_s + strlen(body_s), sizeof(body_s) - strlen(body_s), "\"%s\":%d,", body_key_str, body_value_int);
                }
                body_key = body_key->next;
            }
            if (body_s[strlen(body_s) - 1] == ','){
                body_s[strlen(body_s) - 1] = '}';
                body_s[strlen(body_s)] = '\0';
            }
            body_arr[i] = malloc(strlen(body_s) + 1);
            strcpy(body_arr[i], body_s);
            curl_easy_setopt(handles[i], CURLOPT_POSTFIELDS, body_arr[i]);

        }

        cJSON *connect_timeout_ms = cJSON_GetObjectItem(user->network, "connect_timeout_ms");
        cJSON *timeout_ms = cJSON_GetObjectItem(user->network, "timeout_ms");

        curl_easy_setopt(handles[i], CURLOPT_CONNECTTIMEOUT_MS, connect_timeout_ms->valueint);
        curl_easy_setopt(handles[i], CURLOPT_TIMEOUT_MS, timeout_ms->valueint);
    }

    int index = 0;
    CURLcode res_auth = curl_easy_perform(handles[index]);
        if (res_auth == CURLE_OK){
            curl_easy_getinfo(handles[index], CURLINFO_RESPONSE_CODE, &user->req_metrix->http_code[0]);
            if (user->req_metrix->http_code[0] >= 400) {
                if (log_mod != 0){
                    pthread_mutex_lock(user->mutex_log);
                    char log_msg[256];
                    snprintf(log_msg, sizeof(log_msg), "Thread %d, HTTP %ld", user->id, user->req_metrix->http_code[0]);
                    Logger(user->file_log, log_msg, 0);
                    pthread_mutex_unlock(user->mutex_log);
                }
                user->req_metrix->error_http++;  // ошибка сервера
            } else {
                curl_easy_getinfo(handles[index], CURLINFO_TOTAL_TIME, &user->req_metrix->total_time[0]);
                if (log_mod != 0){
                    pthread_mutex_lock(user->mutex_log);
                    char log_msg[256];
                    snprintf(log_msg, sizeof(log_msg), "Thread %d, HTTP %ld, time %.2f ms", user->id, user->req_metrix->http_code[0], user->req_metrix->total_time[0]);
                    Logger(user->file_log, log_msg, 2);
                    pthread_mutex_unlock(user->mutex_log);
                }
                user->req_metrix->success_http++;
                curl_easy_getinfo(handles[index], CURLINFO_STARTTRANSFER_TIME, &user->req_metrix->ttfb[0]);
                curl_easy_getinfo(handles[index], CURLINFO_NAMELOOKUP_TIME, &user->req_metrix->dns_time[0]);
                curl_easy_getinfo(handles[index], CURLINFO_CONNECT_TIME, &user->req_metrix->connect_time[0]);
                curl_easy_getinfo(handles[index], CURLINFO_SIZE_DOWNLOAD_T, &user->req_metrix->size_down[0]);
            }
            
        }
        else{
            user->req_metrix->error_curl++;
        }
        
    index = 1;
    free(responce.data);
    responce.data = NULL;
    responce.size = 0;
    res_auth = curl_easy_perform(handles[index]);
        if (res_auth == CURLE_OK){
            curl_easy_getinfo(handles[index], CURLINFO_RESPONSE_CODE, &user->req_metrix->http_code[1]);
            if (user->req_metrix->http_code[1] >= 400) {
                if (log_mod != 0){
                    pthread_mutex_lock(user->mutex_log);
                    char log_msg[256];
                    snprintf(log_msg, sizeof(log_msg), "Thread %d, HTTP %ld", user->id, user->req_metrix->http_code[1]);
                    Logger(user->file_log, log_msg, 0);
                    pthread_mutex_unlock(user->mutex_log);
                }
                user->req_metrix->error_http++;  // ошибка сервера
            } else {
                curl_easy_getinfo(handles[index], CURLINFO_TOTAL_TIME, &user->req_metrix->total_time[1]);
                if (log_mod != 0){
                    pthread_mutex_lock(user->mutex_log);
                    char log_msg[256];
                    snprintf(log_msg, sizeof(log_msg), "Thread %d, HTTP %ld, time %.2f ms", user->id, user->req_metrix->http_code[1], user->req_metrix->total_time[1]);
                    Logger(user->file_log, log_msg, 2);
                    pthread_mutex_unlock(user->mutex_log);
                }
                user->req_metrix->success_http++;
                curl_easy_getinfo(handles[index], CURLINFO_STARTTRANSFER_TIME, &user->req_metrix->ttfb[1]);
                curl_easy_getinfo(handles[index], CURLINFO_NAMELOOKUP_TIME, &user->req_metrix->dns_time[1]);
                curl_easy_getinfo(handles[index], CURLINFO_CONNECT_TIME, &user->req_metrix->connect_time[1]);
                curl_easy_getinfo(handles[index], CURLINFO_SIZE_DOWNLOAD_T, &user->req_metrix->size_down[1]);
            }
            
        }
        else{
            user->req_metrix->error_curl++;
        }
    cJSON *json_responce = cJSON_Parse(responce.data);
    if (json_responce == NULL){
        printf("Ошибка JSON. Ошибка Авторизации!\n");
        return NULL;
    }
    cJSON *token_item = cJSON_GetObjectItem(json_responce, "access_token");
    if (token_item == NULL || token_item->valuestring == NULL){
        printf("Ошибка: в ответе нет access_token!\n");
        return NULL;
    }
    char *token_str = token_item->valuestring;


    int time_index = 2;
    int req_count = 1;
    while (1){
        if(time(NULL) - user->time_start >= user->time_work){
            break;
        }
        int value = rand_r(&seed) % weight_sum;
        int sum = 0;
        int index = 0;
        for (int i = 2; i < user->count; i++){
            sum += weight_arr[i];
            if (value < sum){
                index = i;
                sum = 0;
                break;
            }
        }
        if (auth_arr[index]){
            char header_str[1024] = "";
            snprintf(header_str, sizeof(header_str), "Authorization: Bearer %s", token_str);
            headers_arr[index] = curl_slist_append(headers_arr[index], header_str);
            curl_easy_setopt(handles[index], CURLOPT_HTTPHEADER, headers_arr[index]);
            auth_arr[index] = 0;
        }

        if (req_count >= user->req_metrix->max_req) {
            fprintf(stderr, "Thread %d: превышен лимит запросов!\n", user->id);
            break;
        }
        free(responce.data);
        responce.data = NULL;
        responce.size = 0; 

        CURLcode res = curl_easy_perform(handles[index]);
        if (res == CURLE_OK){
            // ПРОВЕРКА: есть ли место для http_code?
            if (req_count + 1 >= user->req_metrix->max_req) {
                fprintf(stderr, "Thread %d: нет места для http_code (req_count=%d)\n", user->id, req_count);
                break;
            }
            req_count++;
            curl_easy_getinfo(handles[index], CURLINFO_RESPONSE_CODE, &user->req_metrix->http_code[req_count]);
            if (user->req_metrix->http_code[req_count] >= 400) {
                if (log_mod != 0){
                    pthread_mutex_lock(user->mutex_log);
                    char log_msg[256];
                    snprintf(log_msg, sizeof(log_msg), "Thread %d, HTTP %ld", user->id, user->req_metrix->http_code[req_count]);
                    Logger(user->file_log, log_msg, 0);
                    pthread_mutex_unlock(user->mutex_log);
                }
                user->req_metrix->error_http++;  // ошибка сервера
            } else {
                curl_easy_getinfo(handles[index], CURLINFO_TOTAL_TIME, &user->req_metrix->total_time[time_index]);
                if (log_mod != 0){
                    pthread_mutex_lock(user->mutex_log);
                    char log_msg[256];
                    snprintf(log_msg, sizeof(log_msg), "Thread %d, HTTP %ld, time %.2f ms", user->id, user->req_metrix->http_code[req_count], user->req_metrix->total_time[time_index]);
                    Logger(user->file_log, log_msg, 2);
                    pthread_mutex_unlock(user->mutex_log);
                }
                user->req_metrix->success_http++;
                curl_easy_getinfo(handles[index], CURLINFO_STARTTRANSFER_TIME, &user->req_metrix->ttfb[time_index]);
                curl_easy_getinfo(handles[index], CURLINFO_NAMELOOKUP_TIME, &user->req_metrix->dns_time[time_index]);
                curl_easy_getinfo(handles[index], CURLINFO_CONNECT_TIME, &user->req_metrix->connect_time[time_index]);
                curl_easy_getinfo(handles[index], CURLINFO_SIZE_DOWNLOAD_T, &user->req_metrix->size_down[time_index]);
                time_index++;
            }
            
        }
        else{
            user->req_metrix->error_curl++;
        }
        usleep(delay_ms_arr[index] * 1000);
    }


    for (int i = 0; i < user->count; i++) {
        if (headers_arr[i] != NULL) {
            curl_slist_free_all(headers_arr[i]);

        }
        if (body_arr[i] != NULL) {
            free(body_arr[i]);
        }
        curl_easy_cleanup(handles[i]);
    }
    free(user);
    free(responce.data);
    cJSON_Delete(json_responce);
    return NULL;
    
}

double Sred_time(double total_time, int all_times_count){
    if (all_times_count == 0){
        printf("Ошибка вычисления среднего времени! Деление на 0!\n");
        return 0;
    }
    return (double)(total_time / all_times_count) * 1000;
}

double Fraction_percent(double total, int total_request){
    if (total_request == 0){
        printf("Ошибка! Деление на 0! Общее количество запросов не должно быть равно 0!\n");
        return 0;
    }
    if (total_request < 0){
        printf("Ошибка! Общее количество запросов не должно быть меньше 0!\n");
        return 0;
    }
    return (double)total / total_request * 100;
}

double P(double *all_times, int all_times_count, double p){
    if (all_times == NULL){
        printf("Ошибка! Массив времени не инициализирован!\n");
        return 0;
    }
    if (p < 0){
        printf("Ошибка! Множитель не может быть меньше 0! Стандартные значения: 0,95 и 0,99!\n");
        return 0;
    }
    return all_times[(int)(all_times_count * p)] * 1000;
}

int main(int argc, char *argv[]){
    int threads = 0;
    int time_work = 0;
    int endpoints = 0;
    int log_mode = 0;

    FILE *file_config = fopen("config", "r");
    if (file_config != NULL){
        char line[256];
        while(fgets(line, sizeof(line), file_config) != NULL){
            if (line[strlen(line) - 1] == '\n'){
                line[strlen(line) - 1] = '\0';
            }
            char *eq = strchr(line, '=');
            if (eq == NULL) {
                continue;
            }
            *eq = '\0';
            char *key = line;
            char *value = eq + 1;
            if (strcmp(key, "THREADS") == 0) {
                threads = atoi(value);
            }
            else if (strcmp(key, "TIME") == 0) {
                time_work = atoi(value);
            }
            else if (strcmp(key, "ENDPOINTS") == 0) {
                endpoints = atoi(value);
            }
            else if (strcmp(key, "LOGGING_ENABLED") == 0) {
                log_mode = atoi(value);
            }
        }
        fclose(file_config);
    }
    if (threads == 0 || time_work == 0 || endpoints == 0){
        if (argc != 4){
            printf("Неверное количество аргументов! Пример: ./main количество-пользователей время-работы(с) количество-эндпоинтев\n");
            return -1;
        }
        threads = atoi(argv[1]);
        time_work = atoi(argv[2]);
        endpoints = atoi(argv[3]);
        log_mode = 0;
    }
    curl_global_init(CURL_GLOBAL_ALL);
    FILE *log = fopen("log.txt", "w");

    FILE* file = fopen("endpoints.json", "r");
    if (file == NULL){
        printf("Ошибка, открытия файла! Проверьте наличие файла \"endpoints.json\".\n");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long position = ftell(file);
    if (position == 0){
        printf("Файл пустой! Заполните файл \"endpoints.json\" в соответствии README.\n");
        return -1;
    }
    rewind(file);

    char *config = malloc(position + 1);
    if (config == NULL){
        printf("Ошибка, выделения памяти!\n");
        return -1;
    }
    fread(config, 1, position, file);
    config[position] = '\0';

    cJSON *properties = cJSON_Parse(config);
    if (properties == NULL){
        printf("Ошибка парсинга JSON! Заполните файл \"endpoints.json\" в соответствии README.\n");
        return -1;
    }

    cJSON *parameters = cJSON_GetObjectItem(properties, "parameters");
    cJSON *host = cJSON_GetObjectItem(parameters, "host");
    cJSON *network = cJSON_GetObjectItem(properties, "network");
    cJSON *requests = cJSON_GetObjectItem(properties, "requests");
    cJSON *req = requests->child;
    cJSON **req_arr = malloc(sizeof(cJSON*) * endpoints);

    int i = 0;
    while(req != NULL){
        req_arr[i] = req;
        req = req->next;
        i++;
    }

    int min_delay_ms = 99999999;
    for (int i = 0; i < endpoints; i++){
        cJSON *curl = req_arr[i];
        cJSON *behavior = cJSON_GetObjectItem(curl, "behavior");
        cJSON *delay_ms = cJSON_GetObjectItem(behavior, "delay_ms");
        if (delay_ms != NULL ){
            if (min_delay_ms > delay_ms->valueint && delay_ms->valueint > 0){
                min_delay_ms = delay_ms->valueint;
            }
        }
        else {
            if (min_delay_ms > 100){
                min_delay_ms = 100;
            }
        }
    }
    int request_count = (time_work*1000) / min_delay_ms + 1000;

    struct RequestMetrics *metrix[threads];
    pthread_t thread[threads];
    pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
    for (int i = 0; i < threads; i++){
        struct ThreadUser *user = malloc(sizeof(struct ThreadUser));
        user->id = i;
        user->time_start = time(NULL);
        user->time_work = time_work;
        user->count = endpoints;
        user->host = host;
        user->req_arr = req_arr;
        user->network = network;
        user->file_log = log;
        user->mutex_log = &log_mutex;
        user->log_mode = log_mode;

        struct RequestMetrics *req_metrix = malloc(sizeof(struct RequestMetrics));
        metrix[i] = req_metrix;
        req_metrix->http_code = malloc(sizeof(long) * request_count);
        req_metrix->total_time = malloc(sizeof(double) * request_count);
        req_metrix->ttfb = malloc(sizeof(double) * request_count);
        req_metrix->dns_time = malloc(sizeof(double) * request_count);
        req_metrix->connect_time = malloc(sizeof(double) * request_count);
        req_metrix->size_down = malloc(sizeof(curl_off_t) * request_count);
        req_metrix->error_curl = 0;
        req_metrix->error_http = 0;
        req_metrix->success_http = 0;
        req_metrix->max_req = request_count;
        user->req_metrix = req_metrix;
        pthread_create(&thread[i], NULL, &ThreadCurl, user);
    }

    for (int i = 0; i < threads; i++){
        pthread_join(thread[i], NULL);
    }

    curl_global_cleanup();
    fclose(log);
    fclose(file);
    cJSON_Delete(properties);
    int total_request = 0;
    int total_succcess = 0;
    for (int i = 0; i < threads; i++){
        total_request+= metrix[i]->success_http + metrix[i]->error_curl + metrix[i]->error_http;
        total_succcess+= metrix[i]->success_http;
    }

    int total_success_http = 0;
    int total_error_http = 0;
    int total_error_curl = 0;
    
    double *all_times = calloc(total_succcess, sizeof(double));
    double *all_ttfb = calloc(total_succcess, sizeof(double));
    double *all_dns = calloc(total_succcess, sizeof(double));
    double *all_connect = calloc(total_succcess, sizeof(double));
    curl_off_t *all_sizes = calloc(total_succcess, sizeof(curl_off_t));;
    int all_times_count = 0;
    double total_time = 0;
    int idx = 0;
    for (int i = 0; i < threads; i++){
        for (int j = 0; j < (metrix[i]->success_http); j++){
            all_times[idx] = metrix[i]->total_time[j];
            all_ttfb[idx] = metrix[i]->ttfb[j];
            all_dns[idx] = metrix[i]->dns_time[j];
            all_connect[idx] = metrix[i]->connect_time[j];
            all_sizes[idx] = metrix[i]->size_down[j];
            total_time += metrix[i]->total_time[j];
            idx++;
            all_times_count++;
        }
    }
    for (int i = 0; i < threads; i++){
    total_success_http += metrix[i]->success_http;
    total_error_http += metrix[i]->error_http;
    total_error_curl += metrix[i]->error_curl;
    }
    qsort(all_times, all_times_count, sizeof(double), Сomparison);
    double sred_time = Sred_time(total_time, all_times_count);
    double success_percent = Fraction_percent(total_success_http, total_request);
    double error_http_percent = Fraction_percent(total_error_http, total_request);
    double error_curl_percent = Fraction_percent(total_error_curl, total_request);
    double p95 = P(all_times, all_times_count, 0.95);
    double p99 = P(all_times, all_times_count, 0.99);

    printf("\n========== РЕЗУЛЬТАТЫ НАГРУЗОЧНОГО ТЕСТА ==========\n");
    printf("Потоков: %d\n", threads);
    printf("Длительность: %d с\n", time_work);
    printf("Всего запросов: %d\n", total_request);
    printf("✅ Успешных: %d (%.2f%%)\n", total_success_http, success_percent);
    printf("❌ HTTP ошибок (4xx/5xx): %d (%.2f%%)\n", total_error_http, error_http_percent);
    printf("❌ Ошибок соединения (curl): %d (%.2f%%)\n", total_error_curl, error_curl_percent);
    printf("\n--- Время ответа (успешные запросы) ---\n");
    printf("Среднее: %.2f мс\n", sred_time);
    printf("p95: %.2f мс\n", p95);
    printf("p99: %.2f мс\n", p99);
    printf("==================================================\n");
    return 0;
}