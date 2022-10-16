#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include <unistd.h>
using namespace std;
using namespace std::chrono;

typedef struct
{
    int* arr;
    int front, rear;
    int size, max_size;
} Queue;

Queue init_queue(int max_size)
{
    Queue new_queue;
    new_queue.arr =new int[max_size];
    new_queue.max_size = max_size;
    new_queue.rear = 0;
    new_queue.front = 0;
    new_queue.size = 0;
    return new_queue;
}
bool empty_queue(Queue q)
{
    return q.size == 0;
}

bool over_queue(Queue q)
{
    return q.size == q.max_size;
}


void enqueue(Queue* q, int val)
{
    if(q->size == q->max_size)
    {
        cout<<"Queue is Full\n"<<endl;
        return;
    }
    q->arr[q->rear] = val;
    q->rear = (q->rear + 1) % q->max_size;
    q->size++;
}
int dequeue(Queue* q)
{
    if(q->size == 0)
    {
        cout<<"Queue is Empty\n"<<endl;
        return -999;
    }
    int temp = q->arr[q->front];
    q->front = (q->front + 1) % q->max_size;
    q->size--;
    return temp;

}

int rand_rang(int max,int min)
{
    return (min+rand()%( max + 1 - min));
}

double rand_ex(double x,double lamda)
{

    double res= -(log(1-x))/lamda;
    return res;
}



Queue queue_wait =init_queue(30);


double interarivle_time=1.0/20.0;
bool check1;
bool check2;
void pause_s1(int n) // pauses the thread until service time is over
{
    high_resolution_clock::time_point clock_start = high_resolution_clock::now();
    check1==0;
    cout << "pause of s1 " <<n<<" seconds" <<endl;
    std::this_thread::sleep_for (std::chrono::seconds(n));
    high_resolution_clock::time_point clock_end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(clock_end - clock_start);
    check1=1;
    cout<<endl<<" task1 dequeued "<<dequeue(&queue_wait)<<endl;
    cout << "Task 1 took " << time_span.count() << " seconds.\n=========================================================\n"<<endl;

}

void pause_s2(int n) // pauses the thread until service time is over
{
    high_resolution_clock::time_point clock_start = high_resolution_clock::now();
    check2=0;
    cout << "pause of s2 " <<n<<" seconds"<<endl;
    std::this_thread::sleep_for (std::chrono::seconds(n));
    high_resolution_clock::time_point clock_end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(clock_end - clock_start);
    check2=1;
    cout<<endl<<" task2 dequeued "<<dequeue(&queue_wait)<<endl;
    cout << "Task 2 took " << time_span.count() << " seconds.\n=========================================================\n"<<endl;
}



int main()
{
    high_resolution_clock::time_point start_c = high_resolution_clock::now();
    srand(time(0));
    for(int i=0; i<30; i++) // fill queue wait w/ tasks
    {

        enqueue(&queue_wait,rand()%30);
        //sleep(interarivle_time);
    }



    double x;
    std::thread task1;
    std::thread task2;
    int f=0;
    while(!empty_queue(queue_wait))
    {

        x=rand()/(RAND_MAX + 1.0);
        int t1=rand_ex(x,1.0/25.0);
        x=rand()/(RAND_MAX + 1.0);
        int t2=rand_ex(x,1.0/25.0);
        int time =max(t1,t2)+1;

        if(f==0)
        {
            task1 = std::thread(pause_s1,t1);
            task1.detach();
            task2 = std::thread(pause_s2,t2);
            task2.detach();
            sleep(time);
            f=1;
        }

        if(check1==1)
        {
            task1 = std::thread(pause_s1,t1);
            task1.detach();
            sleep(t1);
            cout<<endl;

        }
        if(check2==1)
        {
            task2 = std::thread(pause_s2,t2);
            task2.detach();
            sleep(t2);
            cout<<endl;
        }
        if(check1==1&&check2==1)
        {
            cout << "waiting\n";
            std::this_thread::sleep_for (std::chrono::seconds(1));
        }




    }

    high_resolution_clock::time_point end_c = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(end_c - start_c);

    cout << "============================\n============================\nTasks done\nThey took " << time_span.count() << " seconds.";



    return 0;
}
