int main() {

	ThreadPool pool(5);

	for (int i = 0; i < 20; i++)
	{
		cout << "Enquing: " << i << endl;
		pool.Enqueue([i](){
			int sleep = (rand() % 1000000) + 1000000;
			usleep(sleep);
			cout << "Processed: " << i << " Sleep Time: " << sleep << endl;
		});
	}

	return 0;
}