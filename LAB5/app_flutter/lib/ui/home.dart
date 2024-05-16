import 'package:flutter/material.dart';

import '../models/data.dart';
import '../models/data_stream_publisher.dart';

class HomePage extends StatefulWidget {
  const HomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: Center(
        child: Padding(
          padding: EdgeInsets.only(top: 15, left: 15),
          child: StreamBuilder<Data>(
            stream: DataStreamPublisher().getDataStream(),
            builder: (context, snapshot) {
              if (snapshot.hasData && snapshot.data != null) {
                Data data = snapshot.data!;
                return Column(
                  children: <Widget>[
                    Row(
                      children: [
                        Icon(
                          Icons.ac_unit_rounded,
                          size: 60,
                        ),
                        SizedBox(width: 5.0),
                        Text(
                          "Temperature: ${data.temperature}",
                          style: TextStyle(fontSize: 20),
                        ),
                      ],
                    ),
                    SizedBox(height: 15.0),
                    Row(
                      children: [
                        Icon(
                          Icons.water_damage_rounded,
                          size: 60,
                        ),
                        SizedBox(width: 5.0),
                        Text(
                          "Humidity: ${data.humidity}",
                          style: TextStyle(fontSize: 20),
                        ),
                      ],
                    ),
                  ],
                );
              } else if (snapshot.hasError) {
                return Text("Error: ${snapshot.error}");
              } else {
                return CircularProgressIndicator();
              }
            },
          ),
        ),
      ),
    );
  }
}
