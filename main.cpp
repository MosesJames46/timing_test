#include <iostream>
#include <chrono>
#include <random>
#include <unordered_map>


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 generator(seed);


double get_average(std::vector<std::chrono::duration<double>>& a);
std::chrono::duration<double> find_smallest(std::vector<std::chrono::duration<double>>&& a);
std::chrono::duration<double> time_selection(std::vector<int>& a);
std::chrono::duration<double> time_merge(std::vector<int>& a);
std::chrono::duration<double> time_combined(std::vector<int>& a);
void combined_sort(std::vector<int>& a, int beginning, int end);
void merge_sort(std::vector<int>& a);
void divide(std::vector<int>& a, int beginning, int end);
void sort(std::vector<int>& a, int begin, int mid, int end);
void print_array(std::vector<int>& a);
std::vector<int> create_array_int(int amount);
void selection_sort(std::vector<int>& a, int beginning, int end);
std::chrono::duration<double> get_duration(std::chrono::steady_clock::time_point b, std::chrono::steady_clock::time_point e);
std::chrono::steady_clock::time_point time_here();
int gen_rand_int(int lower, int upper);
std::vector<int> partially_sorted_array(int size, float percentage, int randNum);
std::chrono::duration<double> time_combined_large(std::vector<int>& a);
void combined_sort_large(std::vector<int>& a, int beginning, int end);

int main() {
	std::unordered_map<std::string, std::vector<std::chrono::duration<double>>> timed_map;
	timed_map.emplace("selection", std::vector<std::chrono::duration<double>>  {});
	timed_map.emplace("merge", std::vector<std::chrono::duration<double>> {});
	timed_map.emplace("combined", std::vector<std::chrono::duration<double>> {});
	timed_map.emplace("combined_large", std::vector<std::chrono::duration<double>> {});

	double selection_average = 0;
	double merge_average = 0;
	double combined_average = 0;
	double combined_large_average = 0;

	int run_n = 1;
	int n_size = 1000000;

	for (int i = 0; i < run_n; i++) {
		std::vector<int> a = create_array_int(n_size);
		std::vector<int> b = a;
		std::vector<int> c = a;
		std::vector<int> d = a;
		timed_map.at("selection").push_back(time_selection(a));
		double a_average = get_average(timed_map.at("selection"));
		timed_map.at("merge").push_back(time_merge(b));
		double b_average = get_average(timed_map.at("merge"));
		timed_map.at("combined").push_back(time_combined(c));
		double c_average = get_average(timed_map.at("combined"));
		timed_map.at("combined_large").push_back(time_combined(d));
		double d_average = get_average(timed_map.at("combined_large"));
		selection_average += (a_average);
		merge_average += (b_average);
		combined_average += (c_average);
		combined_large_average += (d_average);
	}

	selection_average /= run_n;
	merge_average /= run_n;
	combined_average /= run_n;
	combined_large_average /= run_n;


	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	std::cout << "The average run time for " << n_size << " elements " << run_n << " times in the selection is: " << selection_average << "\n";
	std::cout << "The average run time for " << n_size << " elements " << run_n << " times in the merge is: " << merge_average << "\n";
	std::cout << "The average run time for " << n_size << " elements " << run_n << " times in the combined is: " << combined_average << "\n";
	std::cout << "The average run time for " << n_size << " elements " << run_n << " times in the combined large is: " << combined_large_average << "\n";
	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n\n";

	return 0;
}

std::vector<int> partially_sorted_array(int size, float percentage, int randNum) {
	std::vector<int> a(size);

	for (int i = 0; i < size; i++) {
		a[i] = i;
	}

	int amount = size * percentage;
	for (int i = 1; i < amount; i++) {
		int r1 = gen_rand_int(0, size - 1);
		int r2 = gen_rand_int(0, size - 1);
		int d = a[r1];
		a[r1] = a[r2];
		a[r2] = d;
	}

	return a;
}

int gen_rand_int(int lower, int upper) {
	std::uniform_int_distribution<int> distribution(lower, upper);
	return distribution(generator);
}

std::chrono::steady_clock::time_point time_here() {
	return std::chrono::high_resolution_clock::now();
}

using namespace std::chrono;

std::chrono::duration<double> get_duration(steady_clock::time_point b, steady_clock::time_point e) {
	return std::chrono::duration<double>(e-b);
}

void selection_sort(std::vector<int>& a, int beginning, int end) {
	int minIndex = 0;

	for (int i = beginning; i < end - 1; i++) {
		minIndex = i;
		for (int j = i + 1; j < end; j++) {
			if (a[j] < a[minIndex]) {
				minIndex = j;
			}
		}
		int temp = a[i];
		a[i] = a[minIndex];
		a[minIndex] = temp;
	}
}

std::vector<int> create_array_int(int amount) {
	std::vector<int> a(amount);
	for (int i = 0; i < amount; i++) {
		a[i] = gen_rand_int(0, 1000);
	}
	return a;
}

void print_array(std::vector<int>& a) {
	for (const auto& i : a) {
		std::cout << i << " ";
	}
	std::cout << "\n";
}

void sort(std::vector<int>& a, int begin, int mid, int end) {
	std::vector<int> left(mid - begin + 1, 0);
	std::vector<int> right(end - mid, 0);

	int n = left.size();
	for (int i = 0; i < n; i++) {
		left[i] = a[begin + i];
	}

	n = right.size();
	for (int j = 0; j < n; j++) {
		right[j] = a[j + mid + 1];
	}

	int left_size = left.size();
	int right_size = right.size();
	int i = 0;
	int j = 0;
	int k = begin;

	while ( i < left_size && j < right_size) {
		if (left[i] <= right[j]) {
			a[k] = left[i];
			i++;
		}
		else {
			a[k] = right[j];
			j++;
		}
		k++;
	}

	while (i < left_size) {
		a[k] = left[i];
		i++;
		k++;
	}

	while (j < right_size) {
		a[k] = right[j];
		j++;
		k++;
	}
}

void divide(std::vector<int>& a, int beginning, int end) {
	if (beginning < end) {
		int midpoint = end - ((end - beginning) / 2);
		divide(a, beginning, midpoint);
		divide(a, midpoint + 1, end);
		sort(a, beginning, midpoint, end);
	}
}

void merge_sort(std::vector<int>& a) {
	int size = a.size() - 1;
	divide(a, 0, size);
}

void combined_sort(std::vector<int>& a, int beginning, int end) {
	if ((end - beginning) < 100) {
		selection_sort(a, beginning, end);
		return;
	}
	if (beginning < end) {
		int midpoint = end - ((end - beginning) / 2);
		combined_sort(a, beginning, midpoint);
		combined_sort(a, midpoint + 1, end);
		sort(a, beginning, midpoint, end);
	}
}


void combined_sort_large(std::vector<int>& a, int beginning, int end) {
	if ((end - beginning) < 100) {
		selection_sort(a, beginning, end);
		return;
	}
	if (beginning < end) {
		int midpoint = end - ((end - beginning) / 2);
		combined_sort_large(a, beginning, midpoint);
		combined_sort_large(a, midpoint + 1, end);
		sort(a, beginning, midpoint, end);
	}
}

std::chrono::duration<double> time_selection(std::vector<int>& a) {
	auto t1 = time_here();
	selection_sort(a, 0, a.size() - 1);
	auto t2 = time_here();
	auto dur = get_duration(t1, t2);
	return dur;
}

std::chrono::duration<double> time_merge(std::vector<int>& a) {
	auto t1 = time_here();
	merge_sort(a);
	auto t2 = time_here();
	auto dur = get_duration(t1, t2);
	return dur;
}

std::chrono::duration<double> time_combined(std::vector<int>& a) {
	auto t1 = time_here();
	combined_sort(a, 0, a.size() - 1);
	auto t2 = time_here();
	auto dur = get_duration(t1, t2);
	//std::cout << "The time for this run of combined sort is: " << dur.count() << " s.\n";
	return dur;
}

std::chrono::duration<double> time_combined_large(std::vector<int>& a) {
	auto t1 = time_here();
	combined_sort(a, 0, a.size() - 1);
	auto t2 = time_here();
	auto dur = get_duration(t1, t2);
	//std::cout << "The time for this run of combined sort is: " << dur.count() << " s.\n";
	return dur;
}

std::chrono::duration<double> find_smallest(std::vector<std::chrono::duration<double>>&& a) {
	int smallest = 0;
	for (int i = 0; i < a.size(); i++) {
		if (a[i] < a[smallest]) {
			smallest = i;
		}
	}
	return a[smallest];
}

double get_average(std::vector<std::chrono::duration<double>>& a) {
	double average = 0;
	double n = a.size();
	for (int i = 0; i < n; i++) {
		average += a[i].count();
	}
	return average / n;
}