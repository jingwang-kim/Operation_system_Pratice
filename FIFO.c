#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 12000
#define MAX 20000
#define true 1
#define false 0

int front = -1;
int rear = -1;
int queue[MAX];

int IsEmpty(void) {
	if (front == rear)//front�� rear�� ������ ť�� ����ִ� ���� 
		return true;
	else return false;
}
int IsFull(void) {
	int tmp = (rear + 1) % MAX; //���� ť���� rear+1�� MAX�� ���� ����������
	if (tmp == front)//front�� ������ ť�� ������ �ִ� ���� 
		return true;
	else
		return false;
}

void addq(int p) {
	if (IsFull())
		printf("Queue is Full.\n");
	else {
		rear = (rear + 1) % MAX;
		queue[rear] = p;
	}
}

int deleteq() {
	if (IsEmpty())
		printf("Queue is Empty.\n");
	else {
		front = (front + 1) % MAX;
		return queue[front];
	}
}

void FIFO(int *arr, int frame, int page_num) {
	
	int hit_cnt = 0;
	int fault_cnt = 0;
	int frame_cnt = 0;
	int i = 0;
	int cur = 0;
	int flag = 0;


	while (i != page_num) {
		
		cur = arr[i];

		for (int k = front;k <rear;k++) {
			frame_cnt++;
		}

		if (frame_cnt < frame) {  //queue �������
			
			for (int j = front;j <= rear;j++) {
				if (queue[j] == cur) {
					flag = 1;
					break;
				}
				else
					flag = 0;
			}

			if (flag == 0) { //queue�� �� ������
				addq(cur);
				printf("[%d] Page request %d --> Initial filling \n", i, cur);
				hit_cnt++;
			}

			else {  //queue�� �� ������
				printf("[%d] Page request %d --> Hit \n", i, cur);
				hit_cnt++;
			}
		}

		else { //queue �� ��
			for (int j = front+1;j <= rear;j++) {
				if (queue[j] == cur) {
					flag = 1;
					break;
				}
				else
					flag = 0;
			}

			if (flag == 0) {  //queue�� �� x
				deleteq();
				addq(cur);
				fault_cnt++;
				printf("[%d] Page request %d --> Fault(%d) \n", i, cur,fault_cnt);
			}

			else {   //queue�� �� o
				printf("[%d] Page request %d --> Hit \n", i, cur);
				hit_cnt++;
			}

		}

		i++;
		frame_cnt = 0;
	}

	float ave_hit = (float)hit_cnt / (float)page_num;
	printf("Hit ratio = %.2f (%d/%d)", ave_hit, hit_cnt, page_num);


}

int main(void) {

	FILE* fp = fopen("page_req_10000.txt", "r");
	if (fp == NULL) {
		printf("����");
	}

	//flag = ���μ��� ����
	int frame = 0;
	int page_num = 0;
	fscanf(fp, "%d", &frame);

	fscanf(fp, "%d", &page_num);
	int arr[MAX_LEN];

	int i = 0;
	while (i != page_num) {
		fscanf(fp, "%d", &arr[i]);
		i++;
	}

	FIFO(arr, frame, page_num);

	fclose(fp);
	return 0;
}