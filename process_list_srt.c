#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 12000
#define true 1
#define false 0

typedef struct Prolist {
    int id;       //id
    int atime;    //arrival
    int stime;    //service
    int wtime;    //waiting time
    int rtime;    //response time
}Pro;

typedef struct LOG {
    int lt;     //turnaround time
    int lw;     //waiting time
    int lr;     //response time
}log;

#define MAX 10000
int front = -1;
int rear = -1;
Pro queue[MAX];

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

void addq(Pro p) {
    if (IsFull())
        printf("Queue is Full.\n");
    else {
        rear = (rear + 1) % MAX;
        queue[rear] = p;
    }

}

Pro deleteq() {
    if (IsEmpty())
        printf("Queue is Empty.\n");
    else {
        front = (front + 1) % MAX;
        return queue[front];
    }
}

void Srt(Pro* p, int flag) {

    int t = 0;

    //cpu���� �������� process 
    Pro check;
    check.stime = -1;
    check.id = -1;
    check.atime = -1;
    check.wtime = 0;
    check.rtime = 0;

    //cpu���� Ȯ��
    int pcb = 0;

    //log �����ٸ�
    log l[MAX_LEN];
    l[0].lr = 0;
    l[0].lt = 0;
    l[0].lw = 0;

    int exit = 0;
    int cnt = 0;
    float cpu_u = 0;

    int min_stime;      //���� Ÿ�� ���� ���� ��
    int check_min;      //���� Ÿ�� ���� ���� �Ͱ� �ڸ� ��ȯ
    int check_arrival;  //�����ð� ���� ���� �� üũ
    Pro temp;

    for (int i = 0; i < flag; i++) {
        cpu_u += p[i].stime;
    }

    while (1) {

        //cpu Ȯ��
        if ((check.stime == 0) && (queue[front].id != -1)) {
            exit = t;
            printf("[%d] PID(%d): Exit \n", t, check.id);
            pcb = 0;
            //service = 0;
            cnt++;

            l[check.id].lt = exit - check.atime;
            l[check.id].lw = check.wtime;
            l[check.id].lr = check.rtime - check.atime;

            //���μ��� ���� �� cpu�� �۵��ϴ� �ڸ��� front �ʱ�ȭ
            queue[front].stime = -1;
            queue[front].id = -1;
            queue[front].atime = -1;
            queue[front].wtime = 0;
            queue[front].rtime = 0;

        }

        if (cnt == flag)
            break;

        //arrival �ϳ� ������
        for (int i = 0; i < flag; i++) {
            if (p[i].atime == t) {
                addq(p[i]);
                printf("[%d] PID(%d): NOT-Running \n", p[i].atime, p[i].id);
            }
        }


        //front�� �������� cpu �ڸ�
        if (pcb == 0 && (front != rear)) {
            //���� ���� ���μ��� ����
            check_min = front + 1;
            for (int i = front + 2; i <= rear; i++) {
                if (queue[check_min].stime > queue[i].stime)
                    check_min = i;
            }

            //���� ���� ���μ����� �ߺ� ���̸� ���� ������ �� �����ؾ� �ϹǷ� ���
            int min_num = 0;

            for (int i = front + 1; i <= rear; i++) {
                if (queue[check_min].stime == queue[i].stime)
                    min_num++;
            }

            pcb = 1;

            //
            if (min_num != 1) {
                //���� �����Ѱ� ����
                check_arrival = front + 1;
                for (int i = front + 2; i <= rear; i++) {
                    if (queue[check_arrival].atime > queue[i].atime)
                        check_arrival = i;
                }

                temp = queue[front + 1];
                queue[front + 1] = queue[check_arrival];
                queue[check_arrival] = temp;
            }

            else {
                temp = queue[front + 1];
                queue[front + 1] = queue[check_min];
                queue[check_min] = temp;
            }


            check = deleteq();
            check.stime--;

            if (check.rtime == 0) {
                check.rtime = t;
            }

            printf("[%d] PID(%d): Running ~~~~1\n", t, check.id);
        }

        else if (pcb == 1 && (front != rear)) {
            check_min = front + 1;
            for (int i = front + 2; i <= rear; i++) {
                if (queue[check_min].stime > queue[i].stime)
                    check_min = i;
            }

            if (check.stime > queue[check_min].stime) {
                //���� ���� ���μ����� �ߺ� ���̸� ���� ������ �� �����ؾ� �ϹǷ� ���
                int min_num = 0;

                for (int i = front + 1; i <= rear; i++) {
                    if (queue[check_min].stime == queue[i].stime)
                        min_num++;
                }

                if (min_num != 1) {
                    //���� �����Ѱ� ����
                    check_arrival = front + 1;
                    for (int i = front + 2; i <= rear; i++) {
                        if (queue[check_arrival].atime > queue[i].atime)
                            check_arrival = i;
                    }

                    temp = queue[front + 1];
                    queue[front + 1] = queue[check_arrival];
                    queue[check_arrival] = temp;
                }
                else {
                    temp = queue[front + 1];
                    queue[front + 1] = queue[check_min];
                    queue[check_min] = temp;
                }


                addq(check);
                check = deleteq();
                check.stime--;

                if (check.rtime == 0) {
                    check.rtime = t;
                }

                printf("[%d] PID(%d): Running ~~~~2\n", t, check.id);
            }



            else {
                check.stime--;
                //printf("[%d] PID(%d): Running ~~~~3\n", t, check.id);
            }

        }

        else if (pcb == 1 && (front == rear)) {
            check.stime--;
            //printf("[%d] PID(%d): Running ~~~~4\n", t, check.id);
        }

        else if ((front == rear) && cnt != flag) {
            t++;
            continue;
        }


        for (int i = front; i <= rear; i++) {
            queue[i].wtime++;
        }

        t++;

    }
    printf("\n");

    printf("Log of Process Scheduling \n");
    for (int i = 1; i <= flag; i++) {
        printf("PID(%d) Turnaround Time: %d  Waiting Time : %d   Response Time : %d \n", i, l[i].lt, l[i].lw, l[i].lr);
    }

    float avet = 0;
    float avew = 0;
    float aver = 0;


    for (int i = 1; i <= flag; i++) {
        avet += l[i].lt;
        avew += l[i].lw;
        aver += l[i].lr;
    }

    printf("\n");
    printf("Average Turnaround Time: %.2f \n", avet / flag);
    printf("Average Waiting Time: %.2f \n", avew / flag);
    printf("Average Response Time: %.2f \n", aver / flag);
    printf("CPU Utilization: %.2f%% \n", (cpu_u / t) * 100);

}

int main(void) {

    FILE* fp = fopen("process_list_5.txt", "r");
    if (fp == NULL) {
        printf("����");
    }

    int flag = 0;
    int num1 = 0, num2 = 0, num3 = 0;
    fscanf(fp, "%d", &flag);

    Pro p[MAX_LEN];

    int i = 0;
    while (i != flag) {
        fscanf(fp, "%d %d %d", &p[i].id, &p[i].atime, &p[i].stime);
        p[i].wtime = 0;
        p[i].rtime = 0;

        i++;
    }

    Srt(p, flag);


    fclose(fp);
    return 0;
}