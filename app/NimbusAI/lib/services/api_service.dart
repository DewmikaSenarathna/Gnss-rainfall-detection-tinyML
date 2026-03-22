import 'dart:convert';
import 'package:http/http.dart' as http;
import '../models/sensor_data.dart';

class ApiService {
  static final Uri _endpoint = Uri.parse('http://10.19.40.192/data');
  static const Duration _requestTimeout = Duration(seconds: 8);

  Future<SensorData> fetchData() async {
    try {
      final response = await http.get(_endpoint).timeout(_requestTimeout);

      if (response.statusCode == 200) {
        final dynamic decoded = json.decode(response.body);
        if (decoded is! Map<String, dynamic>) {
          throw const FormatException('Unexpected API response format.');
        }
        return SensorData.fromJson(decoded);
      }

      throw Exception('API error (${response.statusCode}).');
    } on FormatException {
      rethrow;
    } on Exception catch (error) {
      throw Exception('Failed to fetch sensor data: $error');
    }
  }
}