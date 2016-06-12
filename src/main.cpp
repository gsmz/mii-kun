#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <algorithm>
using namespace std;

const int thr_time = 100;
const int thr_congestion_degree = 100;
const int thr_transfer_cnt = 2;
class State{
public:
	int pos_;
	int time_;
	int route_id_;
	int transfer_cnt_;
	vector<int> via_;
	int from_station_id_;
	State(int pos, int time, int route_id, int transfer_cnt, vector<int> via, int from_station_id): pos_(pos), time_(time), route_id_(route_id), transfer_cnt_(transfer_cnt), via_(via), from_station_id_(from_station_id){

	}
	State(){

	}
	bool operator == (const State &o)const{
		return pos_ == o.pos_;
	}
	bool operator < (const State &o)const{
		if ( time_ > o.time_ ){
			return true;
		} else if ( time_ == o.time_ ){
			return transfer_cnt_ > o.transfer_cnt_;
		}
		return false;
	}
};

class Node{
public:
	string station_name_;
	vector<int> edges_;
	int station_id_;
	Node(int station_name, vector<int> edges, int station_id):station_name_(station_name_), edges_(edges), station_id_(station_id){

	}
	Node(){

	}
	void addEdge(const int edge){
		edges_.push_back(edge);
	}
};
class Edge{
public:
	int edge_id_;
	int time_;
	int route_id_;
	vector<int> time_zone_congestion_degree_;
	int src_station_id_;
	int dst_station_id_;
	bool starting_station_flag_;
	Edge(int edge_id, int time, int route_id, vector<int> time_zone_congestion_degree, int src_station_id, int dst_station_id, bool starting_station_flag):edge_id_(edge_id), time_(time), route_id_(route_id), time_zone_congestion_degree_(time_zone_congestion_degree), src_station_id_(src_station_id), dst_station_id_(dst_station_id), starting_station_flag_(starting_station_flag){

	}
	Edge(){

	}
};
bool isVisited[10];
vector<Node> nodes;
vector<Edge> edges;
vector<string> split(const string &str, char delim){
  vector<string> res;
  size_t current = 0, found;
  while((found = str.find_first_of(delim, current)) != string::npos){
    res.push_back(string(str, current, found - current));
    current = found + 1;
  }
  res.push_back(string(str, current, str.size() - current));
  return res;
}
vector<int> convertStringVecToIntVec(const vector<string> &v){
	vector<int> res;
	for(int idx = 0; idx < v.size(); ++idx){
		res.push_back(atoi(v[idx].c_str()));
	}
	return res;
}
void init(){
	fill(isVisited, isVisited+10, 0);
	{
		ifstream ifs("nodelist.txt");
		string line;
		while(getline(ifs, line)){
			vector<string> v = split(line, ' ');
			if ( v.size() != 3 ) cout << "invalid line!!" << endl;
			vector<string> st = split(v[1], ',');
			nodes.push_back(Node(atoi(v[0].c_str()), convertStringVecToIntVec(st), atoi(v[2].c_str())));
		}
	}
	{
		ifstream ifs("edgelist.txt");
		string line;
		while(getline(ifs, line)){
			vector<string> v = split(line, ' ');
			if ( v.size() != 7 ) cout << "invalid line!!" << endl;
			vector<string> st = split(v[3], ',');
			edges.push_back(Edge(atoi(v[0].c_str()), atoi(v[1].c_str()), atoi(v[2].c_str()), convertStringVecToIntVec(st), atoi(v[4].c_str()), atoi(v[5].c_str()), atoi(v[6].c_str())));
		}
	}
}
bool myoperator(const State& a, const State& b){
	return a.pos_ < b.pos_;
}
bool isStartingStation(int target_statation_id, int from_station_id){
	cout << "  " << target_statation_id << "->" << from_station_id << ": ";
	const Node target = nodes[target_statation_id];
	for( int idx_edge = 0; idx_edge < target.edges_.size(); ++idx_edge ){
		if( edges[target.edges_[idx_edge]].dst_station_id_ == from_station_id ){
			if ( edges[target.edges_[idx_edge]].starting_station_flag_ ){
				cout << "start";
			}
			cout << endl;
			return edges[target.edges_[idx_edge]].starting_station_flag_;
		}
	}
	cout << endl;
	return false;
}
bool isStartingStation(const Node& target, const Node& from){
	return isStartingStation(target.station_id_, from.station_id_);
}
void settingStartStation(int station_id){

}
int main(){
	init();
	State n;
	priority_queue<State> pq;
	// TODO: [begin]settiongStartStation	
	pq.push(State(3, 0, -1, -1, vector<int>(), -1));
	pq.push(State(3, 0, -1, -1, vector<int>(), -1));
	// TODO: [end]settiongStartStation	
	vector<State> result;
	while(!pq.empty()){
		State cur_state = pq.top(); pq.pop();
//		if ( isVisited[cur_state.pos_] )continue;
		isVisited[cur_state.pos_] = true;
		cout << "current: " << cur_state.pos_ << endl;
		const Node* cur_node = &nodes[cur_state.pos_];
		{
			/*
			vector<State>::iterator itr = find(result.begin(), result.end(), cur_state);
			if(itr != result.end()){//, && itr != result.end()){
				if(isStartingStation(cur_state.pos_, cur_state.from_station_id_)){
					*itr = cur_state;
				}
				else if(itr->time_ > cur_state.time_){
					*itr = cur_state;
				}
			}else{ 
				result.push_back(cur_state);
			}
			*/
			result.push_back(cur_state);
		}
		for (int idx = 0; idx < cur_node->edges_.size(); ++idx){
			cout << "    " << edges[cur_node->edges_[idx]].edge_id_ << "(" << edges[cur_node->edges_[idx]].time_ << ")[" << edges[cur_node->edges_[idx]].src_station_id_ << " -> " << edges[cur_node->edges_[idx]].dst_station_id_ << "]" <<  endl;
			if ( !isStartingStation(edges[cur_node->edges_[idx]].dst_station_id_, cur_node->station_id_ )){
				//if ( isVisited[edges[cur_node->edges_[idx]].dst_station_id_] )continue;
			}
			cout << "  a" << endl; 
			if (cur_state.from_station_id_ == edges[cur_node->edges_[idx]].dst_station_id_)continue;
			cout << "  b" << endl; 
			if ( cur_state.time_+edges[cur_node->edges_[idx]].time_ > thr_time)continue;
			cout << "  c" << endl; 
			int transfer_cnt = cur_state.transfer_cnt_;
			int route_id = cur_state.route_id_;
			vector<int> via = cur_state.via_;
			via.push_back(edges[cur_node->edges_[idx]].src_station_id_);
			if ( cur_state.route_id_ != edges[cur_node->edges_[idx]].route_id_){
				route_id = edges[cur_node->edges_[idx]].route_id_;
				if ( cur_state.transfer_cnt_ + 1 > thr_transfer_cnt ) continue;
				++transfer_cnt;
//				via.push_back(edges[cur_node->edges_[idx]].src_station_id_);
			}
			cout << "        [ok]" << endl;
			pq.push(State(edges[cur_node->edges_[idx]].dst_station_id_, cur_state.time_+edges[cur_node->edges_[idx]].time_, route_id, transfer_cnt, via, cur_state.pos_));
		}
	}
	cout << "-----Result-----" << endl;
	sort(result.begin(), result.end(), myoperator);
	for( int idx = 0; idx < result.size(); ++idx ){
		cout << ": " << result[idx].pos_ << "([time]:" << result[idx].time_ << ")([line]:" << result[idx].route_id_ << ")([transfer_cnt]:" << result[idx].transfer_cnt_ << ")([via: ";
		for( int idx_via = 0; idx_via < result[idx].via_.size(); idx_via++ ){
			if(idx_via) cout << "-";
			cout << result[idx].via_[idx_via];
		}
		cout << ")" << endl;
	}
	return 0;
}