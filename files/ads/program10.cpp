// Program to implement the following graph traversal techniques.
// a)Depth first search b) Breadth first search.

#include <stdio.h>

#define MAX 10

int adj[MAX][MAX];
int visited[MAX];
int n;

// DFS function
void dfs(int node)
{
    int i;
    printf("%d ", node);
    visited[node] = 1;

    for(i = 0; i < n; i++)
    {
        if(adj[node][i] == 1 && visited[i] == 0)
        {
            dfs(i);
        }
    }
}

// BFS function
void bfs(int start)
{
    int queue[MAX], front = 0, rear = 0;
    int i;

    queue[rear++] = start;
    visited[start] = 1;

    while(front < rear)
    {
        int node = queue[front++];
        printf("%d ", node);

        for(i = 0; i < n; i++)
        {
            if(adj[node][i] == 1 && visited[i] == 0)
            {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
}

int main()
{
    int i, j, start, choice;

    printf("Enter number of nodes: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix:\n");
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            scanf("%d", &adj[i][j]);
        }
    }

    printf("Enter starting node: ");
    scanf("%d", &start);

    printf("1. DFS\n2. BFS\nEnter choice: ");
    scanf("%d", &choice);

    // reset visited
    for(i = 0; i < n; i++)
        visited[i] = 0;

    if(choice == 1)
    {
        printf("DFS traversal: ");
        dfs(start);
    }
    else if(choice == 2)
    {
        printf("BFS traversal: ");
        bfs(start);
    }
    else
    {
        printf("Invalid choice");
    }

    return 0;
}