// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "jsonTest.h"


using bsoncxx::builder::basic::kvp;

//数据格式
/*{
	"name":"MongoDB",
	"json" : {
		"x":1,
		"y":"2",
		"array":["arr1","ar2","ar3"]
	}
}*/


void  jsonTest::insertTest1() {
	// 以<<插入的方式构建json
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder << "name" << "MongoDB"
		<< "json" << bsoncxx::builder::stream::open_document
		<< "x" << 1
		<< "y" << "2"
		<< "array" << bsoncxx::builder::stream::open_array
		<< "ar1" << "ar2" << "ar3"
		<< close_array
		<< bsoncxx::builder::stream::close_document
		<< bsoncxx::builder::stream::finalize;
	bsoncxx::document::view view = doc_value.view();
	//插入数据至mongodb
	optional<mongocxx::result::insert_one> result = mongodb.insert_one("DBname", "CollName", view);
}

void  jsonTest::insertTest2() {
	// 以document文档的形式构建json

	const auto elements = { 1,2,3 };
	auto x = bsoncxx::builder::basic::array{};
	for (const auto& element : elements) {
		x.append(element);
	}

	const auto elements1 = { 11,21,13 };
	auto y = bsoncxx::builder::basic::array{};
	for (const auto& element : elements1) {
		y.append(element);
	}

	time_t myt = time(NULL);
	bsoncxx::document::value document = make_document(kvp("name", "Xiao Ming"), kvp("height", 10), kvp("date", bsoncxx::types::b_date{ std::chrono::system_clock::from_time_t(myt) }),
		kvp("array", make_document(kvp("x", x.view()), kvp("y", y.view()))));

	bsoncxx::document::view view = document.view();

	//插入数据至mongodb
	optional<mongocxx::result::insert_one> result = mongodb.insert_one("DBname", "CollName", view);
}

void jsonTest::insertDateStamp() {
	//获取当前得时间戳
	time_t myt = time(NULL);
	//以NumberLong方式插入时间戳
	bsoncxx::document::value doc = bsoncxx::builder::basic::make_document(
		bsoncxx::builder::basic::kvp("date", myt));
	mongodb.insert_one("DBname", "CollName", doc.view());

	bsoncxx::document::value filter = make_document(kvp("date", make_document(kvp("$lte", bsoncxx::types::b_date{ std::chrono::system_clock::from_time_t(myt)
		}))));

	optional<mongocxx::cursor> cursor =
		mongodb.find("DBname", "CollName", filter.view());

	for (auto&& doc : *cursor) {
		cout << bsoncxx::to_json(doc) << endl;
	}
}

void jsonTest::insertISODate() {
	time_t myt = time(NULL);
	//插入ISODate  isodate是mongodb系统自带的data类型
	bsoncxx::document::value doc = bsoncxx::builder::basic::make_document(
		kvp("date", bsoncxx::types::b_date{ std::chrono::system_clock::from_time_t(myt) }));
	mongodb.insert_one("test", "test", doc.view());
}

time_t standard_to_stamp(const char *str_time);


void jsonTest::findDate() {
	//以标准时间查询
	bsoncxx::document::value filter = make_document(kvp("date",
		make_document(kvp("$gt", bsoncxx::types::b_date{
			std::chrono::system_clock::from_time_t(standard_to_stamp("2019:03:20 11:22:30")) }))));
	optional<mongocxx::cursor> cursor = mongodb.find("test", "test", filter.view());

	for (auto&& doc : *cursor) {
		cout << bsoncxx::to_json(doc) << endl;
	}

	//将标准时间转化为时间戳   传入时间若是北京时间则为"2019:03:20 11:22:30"   
	//若是utc时间则为"2019:03:20 03:22:30Z"  Z是大写
}



void jsonTest::deleteOne() {
	auto result = mongodb.delete_one("DBname", "CollName", make_document(kvp("name", "Xiao Ming")));
}

void jsonTest::deleteAll() {
	auto result = mongodb.delete_many("DBname", "CollName", make_document(kvp("name", "Xiao Ming")));
}

void jsonTest::updtaeOne() {
	mongocxx::options::update update_options;	// 文档更新选项
// true:如果不存在符合条件的文档则会新建一个文档,false:反之
	update_options.upsert(true);

	// 修改第一个符合条件（i = 1)的文档,使 i = 10, 如果不存在符合条件的文档则会新建一个文档
	auto result = mongodb.update_one("DBname", "CollName",
		make_document(kvp("name", "Xiao Ming")),
		make_document(kvp("$set", make_document(kvp("name", "uodateName")))),
		update_options
	);

}

void jsonTest::updateAll() {
	// 使所有i < 100的文档中 i 的值加 100


	auto filter = make_document(kvp("name", "updateName"));
	auto update_value = make_document(kvp("$set", make_document(kvp("name", "Xiao Ming"))));
	auto result = mongodb.update_many("DBname", "CollName", filter.view(),
		update_value.view());

	/*若要更新文档中的内容，需列出文档中的所有值
	如"json" : {
		"x":1,
			"y" : "2",
			"array" : [1, 2, 3]
	}*/

	/*const auto elements = { 1,2,3 };
	auto array_builder = bsoncxx::builder::basic::array{};
	for (const auto& element : elements) {
		array_builder.append(element);
	}
	auto filter = make_document(kvp("json", make_document(kvp("x", 1), kvp("y", "2"), kvp("array", array_builder.view()))));
	auto update_value = make_document(kvp("$set", make_document(kvp("json", make_document(kvp("x", 10), kvp("y", "20"), kvp("array", array_builder.view()))))));
	auto result = mongodb.update_many("test", "test", filter.view(),
		update_value.view());*/
}


void jsonTest::findAll() {
	//从数据库中获取多条数据
	optional<mongocxx::cursor> cursor = mongodb.find("DBname", "CollName", make_document(kvp("name", "Xiao Ming")));

	for (auto&& doc : *cursor) {
		std::cout << bsoncxx::to_json(doc) << std::endl;
	}
}

void jsonTest::findOne() {
	//从数据库中获取一条数据
	optional<bsoncxx::document::value> result = mongodb.find_one("DBname", "CollName", make_document(kvp("name", "Xiao Ming")));

	bsoncxx::document::value value = *result;
	bsoncxx::document::view doc = value.view();

	//解析获取到的数据
	cout << bsoncxx::to_json(doc) << endl;
	cout << doc["name"].get_utf8().value.to_string() << endl;
	bsoncxx::document::view json = doc["array"].get_document().value;
	//cout << json["x"].get_int32().value << "" << endl;
	/*if (bsoncxx::to_string(json["y"].type()) == "int32") {
		cout << json["y"].get_int32().value << "int32" << endl;
	}
	else
		cout << json["y"].get_utf8().value.to_string() << "" << endl;*/
	auto array = json["x"].get_array().value;
	for (bsoncxx::array::element var : array)
	{
		cout << var.get_int32().value << "" << endl;
	}

}

void  jsonTest::fuzzyQuery() {
	//以文档的形式查询
	//bsoncxx::document::value filter = make_document(kvp("name",
	//	make_document(kvp("$regex", "Xiao"))));
	//optional<mongocxx::cursor> cursor =
	//	mongodb.find("test", "test", filter.view());
	//for (auto&& doc : *cursor) {
	//	cout << bsoncxx::to_json(doc) << endl;
	//	//获取字段的值
	//	string name = doc["name"].get_utf8().value.to_string();
	//	cout << name << endl;
	//}

	//以插入的方式查询
	optional<mongocxx::cursor> cursor =
		mongodb.find("DBname", "CollName",
			document{} << "name" << open_document <<
			"$regex" << "Xiao" << close_document << finalize);
	for (auto&& doc : *cursor) {
		cout << bsoncxx::to_json(doc) << endl;
	}
	/*$regex是mongodb中使用正则表达式语法，除此之外还可以用
	$gt、$gte  、$lt 、 $lte分别表示大于、  大于等于、 小于 、 小于等于。*/
}

void jsonTest::uploadFile() {

	optional<mongocxx::result::gridfs::upload> result =
		mongodb.upload_file_to_gridfs("uploadImage", "../file/uploadPay.jpg", "DBname");

	// 如果result未初始化
	if (!result.is_initialized()) {
		std::cout << "上传失败" << endl;
		return;
	}

	// 如果上传成功
	// 获取文件保存后的id（mongodb自己生成的唯一的id, 类型为BSON ObjectId）
	bsoncxx::types::value id = (*result).id();
	// 保存id到文档中
	mongocxx::options::update update_options;	// 文档更新选项
	update_options.upsert(true);				// 如果文档不存在则会新建一条记录

	// 更新名为DBname数据库中，名为CollName集合中的id为"file_pay_img"的文档的"file_id"字段的值
	mongodb.update_one("DBname", "CollName",
		make_document(kvp("_id", "file_pay_img")),
		make_document(kvp("$set", make_document(kvp("file_id", id)))),
		update_options);

	// 获取名为DBname数据库中，名为CollName集合中的id为"file_pay_img"的文档的"file_id"字段的值
	optional<bsoncxx::document::value> doc = mongodb.find_one("DBname", "CollName", make_document(kvp("_id", "file_pay_img")));
	bsoncxx::document::view view1 = (*doc).view(); // 获取文档的view对象
	bsoncxx::types::value file_id = view1["file_id"].get_value();

	// 从名为DBname数据库中，下载文件
	// bucket为默认的 fs
	mongodb.download_file_from_gridfs("DBname", file_id, "../file/downloadPay.jpg");

}


time_t standard_to_stamp(const char *str_time)
{
	struct tm stm;
	int iY, iM, iD, iH, iMin, iS;

	memset(&stm, 0, sizeof(stm));
	iY = atoi(str_time);
	iM = atoi(str_time + 5);
	iD = atoi(str_time + 8);
	iH = atoi(str_time + 11);
	iMin = atoi(str_time + 14);
	iS = atoi(str_time + 17);

	stm.tm_year = iY - 1900;
	stm.tm_mon = iM - 1;
	stm.tm_mday = iD;
	stm.tm_hour = iH;
	stm.tm_min = iMin;
	stm.tm_sec = iS;

	string s(str_time + 19);
	if (s == "Z")
		return mktime(&stm) + 28800;
	else
		return mktime(&stm);
}


int main()
{
	jsonTest::insertTest1();
	jsonTest::insertTest2();
	jsonTest::deleteOne();
	jsonTest::updtaeOne();
	//jsonTest::updateAll();
	//jsonTest::insertTest2();
	//jsonTest::findAll();
	//jsonTest::findOne();
	//jsonTest::fuzzyQuery();
	//jsonTest::insertDateStamp();
	//jsonTest::insertISODate();
	//jsonTest::findDate();
	//jsonTest::uploadFile();

	printf("开始执行操作\n");
	
	//从数据库中查询结果第一条数据
	optional<bsoncxx::document::value> result = mongodb.find_one("ustb", "curve_csm", make_document(kvp("mat_no", "X22B545118000")));
	bsoncxx::document::value value = *result;
	bsoncxx::document::view doc = value.view();//获取到的数据 
	cout << bsoncxx::to_json(doc) << endl;
	cout << doc["mat_no"].get_utf8().value.to_string() << endl;//解析获取到的数据 


	//bsoncxx::document::view json = doc["json"].get_document().value;
	//cout << json["x"].get_int32().value << "" << endl;
	/*if (bsoncxx::to_string(json["y"].type()) == "int32") {
	cout << json["y"].get_int32().value << "int32" << endl;
	}
	else
	cout << json["y"].get_utf8().value.to_string() << "" << endl;*/
	//auto array = json["array"].get_array().value;
	/*for (bsoncxx::array::element var : array)
	{
		cout << var.get_int32().value << "" << endl;
	}*/
	printf("OK!!!\n");
	getchar();

}


