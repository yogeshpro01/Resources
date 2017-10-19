#ifndef ALGO_H
#define ALGO_H

#include<vector>
#include<stddef.h>
#include<queue>

/*
    Author - Yogesh Aggarwal
    Copyrights 2017 ALGO.API 
*/

template <typename T>
struct node{
    T val;
    T *left , *right;
    node(){
        left = right = NULL;
    }
};

template <typename T>
struct p_node{
    node<T> head;
    node<T> *next;
    p_node(){
        next = NULL;
    }
};

struct t_node{
    int trie[][26];
    int cursize;
    t_node(){
        cursize=1;
    }
};

const int MAXN = 1e6+1;

template <typename T>
T merge(T a, T b){
    return a + b;
}



/*
    SEGMENT TREES
*/

template <typename T>
class segment_tree{
    node<T> *t;

    void build_r(T a[], int l, int r, node<T> *cur) {
        if ( l == r ) {
            cur->val = a[l];
        } else {
            cur->left = new node<T>;
            int mid = (l+r)/2;
            build_r(a,l,mid,cur->left);
            cur->right = new node<T>;
            build_r(a,mid+1,r,cur->right);
            t->val = merge( t->left->val , t->right->val );
        }
    }

    void update_r(int pos, T x, int l, int r, node<T> *cur){
        if ( l == r && l == pos ) {
            cur->val = x;
        } else {
            int mid = (l+r)/2;
            if ( pos <= mid ) {
                update_r(pos,x,l,mid,cur->left);
            } else {
                update_r(pos,x,mid+1,r,cur->right);
            }
            cur->val = merge( cur->left->val , cur->right->val );
        }
    }

    T query_r(int lx, int lr, int l, int r,node<T> *cur){
        if ( lx == l && lr == r) {
            return cur->val;
        } else {
            int mid = (l+r)/2;
            if( lr <= mid ) {
                return query_r(lx,lr,l,mid,cur->left);
            } else if ( lx > mid ) {
                return query_r(lx,lr,mid+1,r,cur->right);
            } else {
                return merge( query_r(lx,mid,l,mid,cur->left) , query_r(mid+1,lr,mid+1,r,cur->right) );
            }
        }
    }

    public:

        void build(T a[], int l, int r) {build_r(a,l,r,t);}

        void update(int pos, int x, int l, int r) {update_r(pos,x,l,r,t);}

        T query(int lx, int lr, int l, int r) {return query_r(lx,lr,l,r,t);}
};

/*
    PERSISTENCE SEGMENT TREE
*/

template <typename T>
class persistence_segment_tree{
    p_node<T> t;

    p_node<T> *get(int x, p_node<T> *t){
        int cur = 0;
        p_node<T> *q = t;
        for(int i=0; i<cur; i++) q = q->next;
        return q;
    }

    p_node<T> *get_last(p_node<T> *t){
        p_node<T> *q = t;
        while(q->next != NULL){
            q = q->next;
        }
        return q;
    }

    void build_r(T a[], int l, int r, node<T> *cur){
        if ( l == r ) {
            cur->val = a[l];
        } else {
            int mid = (l+r)/2;
            cur->left = new node<T>;
            cur->right = new node<T>;
            build_r(a,l,mid,cur->left);
            build_r(a,mid+1,r,cur->right);
            cur->val = merge(cur->left->val , cur->right->val);
        }
    }

    void update_r(int pos, T x, int l, int r, node<T> *prev, node<T> *cur){
        if ( l == r ) {
            cur->val = x;
        } else {
            int mid = (l+r)/2;
            if ( pos <= mid ) {
                cur->left = new node<T>;
                cur->right = prev->right;
                update_r(pos,x,l,mid,prev,cur);
            } else {
                cur->right = new node<T>;
                cur->left = prev->left;
                update_r(pos,x,mid+1,r,prev,cur);
            }
            cur->val = merge(cur->left->val , cur->right->val);
        }
    }

    T query_r(int lx, int lr, int l, int r, node<T> *cur){
        if ( lx == l && lr == r) {
            return cur->val;
        } else {
            int mid = (l+r)/2;
            if( lr <= mid ) {
                return query_r(lx,lr,l,mid,cur->left);
            } else if ( lx > mid ) {
                return query_r(lx,lr,mid+1,r,cur->right);
            } else {
                return merge( query_r(lx,mid,l,mid,cur->left) , query_r(mid+1,lr,mid+1,r,cur->right) );
            }
        }
    }

    public:
        void build(T a[], int l, int r) {build_r(a,l,r,t->head);};
        void update(int pos, int x, int l, int r) {
            p_node<T> c1 = get_last(t); p_node<T> c2 = new p_node<T> ;
            c1->next = c2;
            update_r(pos,x,l,r,c1->head,c2->head);
        };
        T query(int head, int lx, int lr, int l, int r){
            p_node<T> q = get(head,t);
            return query_r(lx,lr,l,r,q->head);
        }
};

/*
    TRIE
*/

class trie{
    t_node t;
    public:

        void insert(const char *s){
            int cur = 0;
            while(*s != '\0'){
                if(t.trie[cur][*s-'a']==-1)
                    cur = t.trie[cur][*s-'a'] = t.cursize++;
                else
                    cur = t.trie[cur][*s-'a'];
                s++;
            }
        }

        bool find(const char *s){
            int cur = 0;
            while(*s != '\0'){
                if(t.trie[cur][*s-'a']==-1)
                    return false;
                else
                    cur = t.trie[cur][*s-'a'];
                s++;
            }
            return true;
        }

};

/*
    BINARY INDEXED TREES
*/

class binary_indexed_tree{
    std::vector<int>tree;
    int maxVal;
    public:

        binary_indexed_tree(int sz, int mval){
            tree.resize(sz);
            maxVal=mval;
        }

        void update(int val, int idx){
            while(idx <= maxVal){
                tree[idx] += val;
                idx += (idx & -idx);
            }
        }

        void build(std::vector<int> a, int n){
            for(int i=1; i<=n; i++){
                update(a[i-1],i);
            }
        }

        int query(int idx){
            int sum = 0;
            while(idx > 0){
                sum += tree[idx];
                idx -= (idx & -idx);
            }
            return sum;
        }
};

/*
    FLOYD-WARSHALL SHORTEST PATH
*/

void floyd_warshall(int graph[][MAXN],int v,int dist[][MAXN]){
    for(int i = 0; i < v; i++){
        for(int j = 0; j < v; j++){
            if( i == j ) dist[i][j] = 0;
            else dist[i][j] = graph[i][j];
        }
    }

    for(int k = 0; k < v; k++){

        for(int i = 0; i < v; i++){

            for(int j = 0; j < v; j++){

                if(dist[i][k] != INT_MAX && dist[k][j] != INT_MAX){
                    if(dist[i][k] + dist[k][j] < dist[i][j]){
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

/*
    DIJKSTRA's SHORTEST PATH
*/

void dijkstra(std::vector< std::pair<int,int> >graph[], int v, int source, int dist[]){
    std::priority_queue< std::pair<int,int> , std::vector< std::pair<int,int> > , std::greater< std::pair<int,int> > > pq;
    for(int i=1; i<=v; i++) dist[i] = INT_MAX;
    int vis[v+1] = { };
    dist[source] = 0;
    pq.push(std::make_pair(dist[source],source));
    while(!pq.empty()){
        int node = pq.top().first , cur = pq.top().second;
        pq.pop();
        if(dist[node] == cur && !vis[node]){
            for(int i=0; i<graph[node].size(); i++){
                int v = graph[node][i].first, d = graph[node][i].second;
                if(cur + d < dist[v]){
                    dist[v] = cur + d;
                    pq.push_back(std::make_pair(dist[v],v));
                }
            }
            vis[node] = 1;
        }
    }
}

/*

*/

#endif

