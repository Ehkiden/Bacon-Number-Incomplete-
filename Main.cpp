//course: CS216-005
//Project: Programming  Assignment 3
//Date: 04/18/2017
//Purpose: To assign actos and the movies they played in into a matrix to measure the "Bacon-number" by using bts algorithm
//Author: Jared Rigdon

//This project is not fully complete and still has a lot of bugs, the test txt files work, just not the full txt files
//Big note: the txt file will look confusing when viewed in notepad. The txt file contains a newline character but for some reason it doesnt show up. 
//			so the actual file will look like 150|63 "newline" 150|3, NOT 150|63150|3
//side note: could make alot of side functions to reduce repeditive function calling but whatever

/*
Purpose: The main purpose of this program is to find the "Bacon Number" of any actor that played in a movie and count how many relations away the chosen actor is from Kevin Bacon
Note: This is largely dependent on the Dijkstra's algorithm, really confusing algthm at that
Basics:	This will first read from multiple txt files the user will "provide". The files will be the actor list, movie list, and actor-movie list
		The txt file will be in the form of "number(aka wieght)" | "name of actor/movie" for the actor and movie list
		The actor-movie list will be in the form of "number of movie" | "number of actor"

		Then create a matrix graph with the given info and assign the weight of the actor to the proper movie edge
		Then perform Dijkstra's alghtm to find the bacon number

*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>   // transform() function
#include <cctype>
#include <set>
#include <map>
#include "Graph.h"

using namespace std;

void print_path(vector<int>& go_through, int j);
bool check_file(string file_name);
void insert_map(map<int, string>&actors_db, map<string, int>&rev_actors_db, string file_name);

int main()
{
	//initializes the maps for the actors and movies database
	map<int, string> actors_db;
	map<int, string> movies_db;
	map<string, int> rev_actors_db;
	map<string, int> rev_movies_db;
	map<int, set<int>> mov_act_db;

	//make the bacon number
	int bacon;

	//get the file
	string actor_txt;
	cout << "Please type the name of the file which contains actor IDs and names: (default file name is \"actors.txt\"):" << endl;
	getline(cin, actor_txt);

	check_file(actor_txt);

	insert_map(actors_db, rev_actors_db, actor_txt);

	map<int, int> actor_vert;
	map<int, int> vert_actor;

	int vert_num = 0;

	//assigns the actor to the vertex
	for (auto i = actors_db.cbegin(); i != actors_db.cend(); ++i) {
		actor_vert.insert(make_pair(vert_num, i->first));
		vert_num++;
	}
	vert_num = 0;
	//assigns the vertext to the actor
	for (auto i = actors_db.cbegin(); i != actors_db.cend(); ++i) {
		vert_actor.insert(make_pair(i->first, vert_num));
		vert_num++;
	}
	for (auto i = actors_db.cbegin(); i != actors_db.cend(); ++i) {
		if (i->second == "Kevin Bacon") {
			bacon = i->first;
			break;
		}
	}
	//get the file
	string movies_txt;
	cout << "Please type the name of the file which contains movie IDs and names: (default file name is \"movies.txt\"):" << endl;
	getline(cin, movies_txt);
	//check_file makes sure the file is there
	check_file(movies_txt);
	insert_map(movies_db, rev_movies_db, movies_txt);

	string movies_actors_txt;
	cout << "Please type the name of the file which contains movie IDs and actor IDs: (default file name is \"movie-actor.txt\"):" << endl;
	getline(cin, movies_actors_txt);

	check_file(movies_actors_txt);

	ifstream infile;
	infile.open(movies_actors_txt);

	while (!infile.eof())
	{
		string actorID_str, movieID_str, line;
		getline(infile, line);
		istringstream iss(line);
		if (line != "") {
			//get movieID
			getline(iss, movieID_str, '|');
			//get actorID
			getline(iss, actorID_str);

			//convert to int
			int movieID = atoi(movieID_str.c_str());
			int actorID = atoi(actorID_str.c_str());

			set<int> actors;

			if (mov_act_db.find(movieID) == mov_act_db.end()) {
				actors.insert(actorID);
				mov_act_db.insert(pair<int, set<int> >(movieID, actors));
			}
			else {
				mov_act_db[movieID].insert(actorID);
			}
		}

	}
	infile.close();
	int y = 1;
	Graph testingGraph(actors_db.size());

	//add the edges to the graph
	for (auto it = mov_act_db.cbegin(); it != mov_act_db.cend(); ++it) {
		for (auto i = it->second.cbegin(); i != it->second.cend(); ++i) {
			for (auto x = next(it->second.cbegin(), y); x != it->second.cend(); ++x) {
				int something = *i;
				auto v = vert_actor.find(something);
				int m = v->second;

				int something_else = *x;
				auto w = vert_actor.find(something_else);
				int n = w->second;

				int b = it->first;
				testingGraph.addEdge(m, n, b);

			}
			y++;
			cout << endl;

		}
		y = 1;
		cout << endl;
	}

	string source_str;

	//Ask the user for the name of the actor
	while (true)
	{
		cout << "Please choose the vertex as the source (type 'Q' or 'q' to quit): " << endl;
		getline(cin, source_str);
		cin.ignore(256, '\n');

		if (source_str == "Q" || source_str == "q")
			break;

		auto new_source = rev_actors_db.find(source_str);
		int source = new_source->second;
		auto new_new_source = actor_vert.find(source);
		source = new_new_source->second;
		cout << endl;
		cout << "Breadth First Traversal "
			<< "(starting from the source vertex: " << source << " )" << endl;
		vector<int> distance(actors_db.size(), -1);
		vector<int> go_through(actors_db.size(), -1);
		testingGraph.BFS(source, distance, go_through);
		// Display the distance of each vertex(except for the source itself) to the source vertex
		// Display the shortest path from each vertex(except for the source itself) to the source 
		// the distance is defined as the number of edges
		// the distance of the source to the source is 0
		// there is no path from the source to a vertex if its distance is -1
		for (int i = 0; i < distance.size(); i++)
		{
			if (i != source)
			{
				if (distance[i] != -1) {
					cout << "The distance from source vertex " << source << " to vertex " << i << " is " << distance[i] << "-edge away." << endl;
					cout << "The path from the source vertex to the vetrex " << i << ": \n" << source;
					print_path(go_through, i);
					cout << endl;
				}
				else
					cout << "there is no path to vertex " << i << endl;
			}
		}
	}

	return 0;
}

void print_path(vector<int>& go_through, int j) {
	if (go_through[j] == -1) {
		return;
	}
	print_path(go_through, go_through[j]);

	cout << "--->" << j;
}
bool check_file(string file_name) {

	ifstream infile;
	infile.open(file_name);
	if (!infile.good())
	{
		cout << "Cannot open the file named " << file_name << endl;
		return 2;
	}
	else {
		infile.close();
		return true;
	}
}

void insert_map(map<int, string> &actors_db, map<string, int>&rev_actors_db, string file_name) {
	ifstream infile;
	infile.open(file_name);

	while (!infile.eof())
	{
		//works
		string fileID_str;
		string file_name, line;
		getline(infile, line);
		istringstream iss(line);
		if (line != "") {
			//get ID
			getline(iss, fileID_str, '|');
			//get name
			getline(iss, file_name);

			//convert to int
			int fileID = atoi(fileID_str.c_str());

			//assign the actorID to the actor name
			actors_db.insert(make_pair(fileID, file_name));
			rev_actors_db.insert(make_pair(file_name, fileID));
		}
	}
	infile.close();
}
