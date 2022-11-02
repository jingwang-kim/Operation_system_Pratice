#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 12000
#define MAX 20000
#define true 1
#define false 0

typedef struct LRU_list {
	int item;
	int cnt;
}lru;

void LRU(lru* arr, int frame, int page_num) {

	int hit_cnt = 0;
	int fault_cnt = 0;
	int frame_cnt = 0;
	int i = 0;
	lru cur;
	lru temp;
	lru queue[MAX];
	cur.item = 0;
	cur.cnt = 0;
	int flag = 0;
	int old = 0;
	int b = 0;

	while (i != page_num) {

		cur = arr[i];

		if (b < frame) {  //queue 비어있음

			for (int j = 0;j < b;j++) {
				if (queue[j].item == cur.item) {
					flag = 1;
					break;
				}
				else
					flag = 0;
			}

			if (flag == 0) { //queue에 값 없으면
				queue[b] = cur;
				printf("[%d] Page request %d --> Initial filling \n", i, cur.item);
				hit_cnt++;
				b++;
			}

			else {  //queue에 값 있으면
				printf("[%d] Page request %d --> Hit \n", i, cur.item);
				for (int k = 0;k < b;k++) {
					if (cur.item == queue[k].item)
						queue[k].cnt = 0;
				}
				hit_cnt++;
			}
		}

		else { //queue 다 참
			for (int j = 0;j < b;j++) {
				if (queue[j].item == cur.item) {
					flag = 1;
					break;
				}
				else
					flag = 0;
			}

			if (flag == 0) {  //queue에 값 x
				for (int k = 0;k < b;k++) {
					if (queue[k].cnt > queue[old].cnt)
						old = k;
				}

				temp = queue[old];
				queue[old] = cur;
				cur = temp;

				fault_cnt++;
				printf("[%d] Page request %d --> Fault(%d) \n", i, cur.item, fault_cnt);
			}

			else {   //queue에 값 o
				printf("[%d] Page request %d --> Hit \n", i, cur.item);
				for (int k = 0;k < b;k++) {
					if (cur.item == queue[k].item)
						queue[k].cnt = 0;
				}
				hit_cnt++;
			}

		}

		i++;
		frame_cnt = 0;
		for (int a = 0;a < b;a++) {
			queue[a].cnt++;
		}
	}

	float ave_hit = (float)hit_cnt / (float)page_num;
	printf("Hit ratio = %.2f (%d/%d)", ave_hit, hit_cnt, page_num);


}

int main(void) {

	FILE* fp = fopen("page_req_10000.txt", "r");
	if (fp == NULL) {
		printf("실패");
	}

	//flag = 프로세스 개수
	int frame = 0;
	int page_num = 0;
	fscanf(fp, "%d", &frame);

	fscanf(fp, "%d", &page_num);
	lru arr[MAX_LEN];

	int i = 0;
	while (i != page_num) {
		fscanf(fp, "%d", &arr[i].item);
		arr[i].cnt = 0;
		i++;
	}

	LRU(arr, frame, page_num);

	fclose(fp);
	return 0;
}