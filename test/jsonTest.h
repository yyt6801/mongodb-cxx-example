#pragma once
#include "MongoDB.h"
#include <iostream>
#include <mongocxx\pool.hpp>
#include <mongocxx\client.hpp>
#include <bsoncxx\json.hpp>
#include <string>
#include <bsoncxx\builder\stream\document.hpp>     


MongoDB mongodb("mongodb://127.0.0.1:27017/?minPoolSize=0&maxPoolSize=10");
using namespace std;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;
using mongocxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;



class jsonTest {
public:
	jsonTest();
	~jsonTest();
	static void insertTest1();
	static void insertTest2();
	static void insertDateStamp();
	static void insertISODate();
	static void deleteOne();
	static void deleteAll();
	static void updtaeOne();
	static void updateAll();
	static void findAll();
	static void findOne();
	static void findDate();
	static void fuzzyQuery();
	static void uploadFile();
	static void downloadFile();
	
};