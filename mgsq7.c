#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSLOT 32749
#define SYMMETRY 1
#define SKIPCOND false //(i == 4 || i == 5 || i == 6)


typedef struct _slot_t {
  char val[18];
  struct _slot_t * next;
} slot_t;

slot_t* H[6][MAXSLOT];
char lines[288000][22];


int fast_strncmp( const char *ptr0, const char *ptr1, int len ){
  for (int i = 0; i < len; i++){
    if (ptr0[i] != ptr1[i]){
      return -1;
    }
  }
  return 0;
}


int ht_hash(char* x, int l){
  if (l == 3){
    return (((int)(unsigned char)x[0]<<16) | ((int)(unsigned char)x[1]<<8) | ((int)(unsigned char)x[2]))%MAXSLOT;
  }
  int y = 0;
  for (int i = 0; i < l; i++){
    int c = (int)(unsigned char)x[i];
    int s = (i % 4)*6;
    y ^= c<<s;
  }
  y %= MAXSLOT;
  return y;
}

int ht_has(slot_t** ht, char* k, int l){
  slot_t* s = ht[ht_hash(k,l)];
  if (!s){
    return 0;
  }
  do{
    if (fast_strncmp(s->val,k,l) == 0){
      return 1;
    }
  }while((s = s->next));
  return 0;
}

void ht_insert(slot_t** ht, char* k, int l){
  int h = ht_hash(k,l);
  // printf("%x\n",h);
  slot_t* s = ht[h];
  slot_t* x;

  if (!s){
    goto lbl_insert;
  }
  while (1){
    if (fast_strncmp(s->val,k,l) == 0){
      return;
    }
    if (s->next){
      s = s->next;
    }else{
      break;
    }
  }
  lbl_insert:

  x = (slot_t*)calloc(sizeof(slot_t),1);
  memcpy(x->val,k,l);
  if (s){
    s -> next = x;
  }else{
    ht[h] = x;
  }
}

void ht_check(slot_t** ht){
  for (int i = 0; i < MAXSLOT; i++){
    int cnt = 0;
    slot_t* s = ht[i];
    while (s != 0){
      s = s->next;
      cnt ++;
    }
    if (!cnt){
      printf("_");
    }else{
      printf("(%x)",cnt);
    }
  }
  printf("\n");
}


int main(int argc, char **argv){

  FILE * fp;
  char line[22];
  size_t len = 0;

  if (argc >= 2){
    fp = fopen(argv[1], "r");
  }else{
    fp = fopen("7.txt", "r");
  }

  if (fp == NULL)
    exit(1);

  int n_lines = 0;
  while ( fgets (line, 22, fp)!=NULL ) {
    if (strlen(line)!=21){
      continue;
    }
    for (int i = 0; i < 6; i++){
      ht_insert(H[i],line,3*(i+1));
    }
    memcpy(lines[n_lines],line,21);
    n_lines ++;
  }
  printf("#lines: %d\n",n_lines);

  fclose(fp);

  // ht_check(H[4]);
  // exit(0);

  int L0 = 0;
  int L1 = n_lines;

  if (argc >= 3){
    L0 = atoi(argv[2]);
    if (argc >= 4){
      L1 = atoi(argv[3]);
    }
  }
  
  char q[7][21] = {0};

  for (int a = L0; a < L1; a++){
    if (!(a % 100)) printf("%6d/[%6d,%6d]\n",a,L0,L1);

    for (int i = 0; i < 7; i++){
      memcpy(q[i],lines[a]+i*3,3);
      if (SKIPCOND) continue;

      if (!ht_has(H[0],q[i],3)){
        goto lbl_next_a;
      }
    }

    for (int b = 0; b < n_lines; b++){
#if SYMMETRY
      if (fast_strncmp(lines[b],lines[a]+3,3)!=0){
        goto lbl_next_b;
      }
#endif
      for (int i = 0; i < 7; i++){
        memcpy(q[i]+3,lines[b]+i*3,3);
        if (SKIPCOND) continue;

        if (!ht_has(H[1],q[i],6)){
          goto lbl_next_b;
        }
      }

      // printf("->%6d %s %s\n",a,lines[a],lines[b]);


      for (int c = 0; c < n_lines; c++){
#if SYMMETRY
        if (fast_strncmp(lines[c],lines[a]+6,3)!=0 || fast_strncmp(lines[c]+3,lines[b]+6,3)!=0){
          goto lbl_next_c;
        }
#endif

        for (int i = 0; i < 7; i++){
          memcpy(q[i]+6,lines[c]+i*3,3);
          if (SKIPCOND) continue;

          if (!ht_has(H[2],q[i],9)){
            goto lbl_next_c;
          }
        }

        printf("->%6d %s %s %s\n",a,lines[a],lines[b],lines[c]);
        
        for (int d = 0; d < n_lines; d++){

#if SYMMETRY
          if (fast_strncmp(lines[d],lines[a]+9,3)!=0 || fast_strncmp(lines[d]+3,lines[b]+9,3)!=0 || fast_strncmp(lines[d]+6,lines[c]+9,3)!=0){
            goto lbl_next_d;
          }
#endif
          for (int i = 0; i < 7; i++){

            memcpy(q[i]+9,lines[d]+i*3,3);
            if (SKIPCOND) continue;
 
            if (!ht_has(H[3],q[i],12)){
              goto lbl_next_d;
            }
          }

          // printf("->%6d %s %s %s %s\n",a,lines[a],lines[b],lines[c],lines[d]);

          for (int e = 0; e < n_lines; e++){

            for (int i = 0; i < 7; i++){

              memcpy(q[i]+12,lines[e]+i*3,3);
              if (SKIPCOND) continue;

              if (!ht_has(H[4],q[i],15)){
                goto lbl_next_e;
              }
            }

            // printf("->%6d %s %s %s %s %s\n",a,lines[a],lines[b],lines[c],lines[d],lines[d]);

            for (int f = 0; f < n_lines; f++){

              for (int i = 0; i < 7; i++){

                memcpy(q[i]+15,lines[f]+i*3,3);
                if (SKIPCOND) continue;

                if (!ht_has(H[5],q[i],18)){
                  goto lbl_next_f;
                }
              }

              printf("------------------------------------------FOUND------------------------------------------\n");
              printf("%s %s %s %s %s %s\n",lines[a],lines[b],lines[c],lines[d],lines[e],lines[f]);
              printf("-----------------------------------------------------------------------------------------\n");


              lbl_next_f: continue;
            }
            lbl_next_e: continue;
          }
          lbl_next_d: continue;
        }
        lbl_next_c: continue;
      }
      lbl_next_b: continue;
    }
    lbl_next_a: continue; 
  }

  exit(0);
}
