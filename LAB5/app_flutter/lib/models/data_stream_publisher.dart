import 'package:firebase_database/firebase_database.dart';

import 'data.dart';

class DataStreamPublisher {
  final _database = FirebaseDatabase.instance.ref();
  Stream<Data> getDataStream() {
    final dataStream = _database.child('data').onValue;
    final streamToPublish = dataStream.map((event) {
      final dataMap = event.snapshot.value as Map<dynamic, dynamic>;
      return Data.fromRTDB(dataMap);
    });
    return streamToPublish;
  }
}
