
#include <windows.h>
#include <iostream>
#include <map>

#include "../thread_safe_map.h"

using namespace ctle;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

template<class T> T random_value();

// just some value
const i64 global_random_seed = 12876123876;

inline void setup_random_seed()
	{
	i64 seed = (global_random_seed == -1) ? i64( time( nullptr ) ) : global_random_seed;
	srand(u32( seed & 0xffffffff ));
	}

// add headers that you want to pre-compile here
inline u8 u8_rand() { return (u8)(rand() & 0xff); } 
inline u16 u16_rand() { return (rand() << 4) ^ rand(); } 
inline u32 u32_rand() { return u32(u16_rand()) << 16 | u32(u16_rand()); } 
inline u64 u64_rand() { return u64(u32_rand()) << 32 | u64(u32_rand()); } 

template <> bool random_value<bool>() { return (bool)(rand() & 0x1); } 

template <> i8 random_value<i8>() { return u8_rand() ; }
template <> i16 random_value<i16>() { return u16_rand() ; }
template <> i32 random_value<i32>() { return u32_rand() ; }
template <> i64 random_value<i64>() { return u64_rand() ; }

template <> u8 random_value<u8>() { return u8_rand() ; }
template <> u16 random_value<u16>() { return u16_rand() ; }
template <> u32 random_value<u32>() { return u32_rand() ; }
template <> u64 random_value<u64>() { return u64_rand() ; }

template<class _MapTy, size_t levelsToTest> 
class threadedMapPerfTesting
	{
	private:
		std::vector<u64> random_vector;

		_MapTy testMap;

		const size_t itemCount_thread_safe_map = 2048; 
		const size_t passCount_thread_safe_map = 10; 

		void testThread() 
			{
			for( size_t pass = 0; pass < passCount_thread_safe_map; ++pass )
				{
				// insert all values	
				for( size_t inx = 0; inx < itemCount_thread_safe_map; ++inx )
					{
					const u64 value = random_vector[inx];
					testMap.insert( std::pair<u64, u64>( value, value ) );
					}

				// remove all values	
				for( size_t inx = 0; inx < itemCount_thread_safe_map; ++inx )
					{
					const u64 value = random_vector[inx];
					testMap.erase( value );
					}
				}
			}

		static void testEntry( void *pthis )
			{
			threadedMapPerfTesting *ptr = (threadedMapPerfTesting *)pthis;
			ptr->testThread();
			}

		void run()
			{
			std::cout << "threadedMapPerfTesting:" << std::endl;

			// setup random vector
			random_vector.resize( itemCount_thread_safe_map );
			for( size_t inx = 0; inx < itemCount_thread_safe_map; ++inx )
				{
				random_vector[inx] = random_value<u64>();
				}

			// test the number of levels specified
			for( size_t lev = 0; lev < levelsToTest; ++lev )
				{
				testMap.clear();

				std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

				// setup and run all threads
				const size_t num_threads = ((size_t)1 << lev);
				std::vector<std::thread> threads(num_threads);
				for (size_t i = 0; i < num_threads; i++) 
					{
					threads[i] = std::thread(testEntry, this);
					}

				// wait for all threads to complete
				for (auto& th : threads) 
					{
					th.join();
					} 

				std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

				double timeSpan_s = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
				double perIterationTime_us = timeSpan_s / double( num_threads ) / double( itemCount_thread_safe_map*passCount_thread_safe_map ) * double(1000000);

				std::cout << "\tRunning " << num_threads << " threads took " << timeSpan_s << " secs.\tTime per iteration: " << perIterationTime_us << " usecs." << std::endl;
				}
			}

	public:
		static void runTest()
			{
			threadedMapPerfTesting tst;
			tst.run();
			}

	};

int main()
	{
	threadedMapPerfTesting<std::unordered_map<u64, u64>, 1>::runTest();
	threadedMapPerfTesting<std::map<u64, u64>, 1>::runTest();
	threadedMapPerfTesting<ctle::thread_safe_map<u64, u64>, 8>::runTest();


	}

