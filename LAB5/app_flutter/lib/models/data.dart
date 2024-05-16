class Data {
  int temperature;
  int humidity;

  Data({required this.temperature, required this.humidity});

  factory Data.fromRTDB(Map<dynamic, dynamic> data) {
    return Data(
      temperature: data['temperature'] ?? 0,
      humidity: data['humidity'] ?? 0,
    );
  }
}
