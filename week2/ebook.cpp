#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>
#include <numeric>

using namespace std;

class ReadingManager {
	map<int, int> user_pages;
	vector<int> page_users;
public:
   	ReadingManager() : page_users(1000, 0) {};

  	void Read(int user_id, int page_count) {
		if (!user_pages.count(user_id)) {
			for (int i = 0; i < page_users.size(); ++i) {
				if (i > page_count - 1) {
					++page_users[i];
				}
			}
		}
		else {
			for (int i = 0; i < page_users.size(); ++i) {
				if (i < page_count && i >= user_pages[user_id]) {
					--page_users[i];
				}
			}
		}
		// for (int i = 0; i < 20; ++i) {
		// 	cout << " " << page_users[i];
		// }
		// cout << endl;
		user_pages[user_id] = page_count;
  	}

	double Cheer(int user_id) const {
		if (!user_pages.count(user_id)) {
			return 0;
		}
		else if (user_pages.size() == 1) {
			return 1;
		}
		else {
			double users = page_users[user_pages.at(user_id) - 1];
			// cout <<  "users " << users  << endl;

			return users / (user_pages.size() - 1);
		}
	}
};


int main() {
	ReadingManager manager;
	int query_count;

	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		int user_id;

		cin >> query_type >> user_id;

		if (query_type == "READ") {
			int page_count;

			cin >> page_count;
			manager.Read(user_id, page_count);
		}
		else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}

  return 0;
}


