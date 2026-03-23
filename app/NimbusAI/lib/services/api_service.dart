import 'dart:convert';
import 'dart:async';
import 'package:http/http.dart' as http;

import '../config/app_config.dart';
import '../models/sensor_data.dart';

class ApiService {
  static final List<Uri> _endpoints = AppConfig.sensorApiEndpoints
      .split(',')
      .map((value) => value.trim())
      .where((value) => value.isNotEmpty)
      .map(Uri.parse)
      .toList(growable: false);
  static const Duration _requestTimeout = Duration(seconds: 10);
  static const int _maxAttempts = 2;

  Future<SensorData> fetchData() async {
    Object? lastError;

    for (var attempt = 1; attempt <= _maxAttempts; attempt++) {
      for (final endpoint in _endpoints) {
        try {
          final response = await http.get(endpoint).timeout(_requestTimeout);

          if (response.statusCode == 200) {
            final dynamic decoded = json.decode(response.body);
            if (decoded is! Map<String, dynamic>) {
              throw const FormatException('Unexpected API response format.');
            }
            return SensorData.fromJson(decoded);
          }

          throw Exception('API error (${response.statusCode}) from $endpoint.');
        } on TimeoutException {
          lastError = Exception(
            'Timeout while contacting sensor endpoint $endpoint.',
          );
        } on FormatException {
          rethrow;
        } on Exception catch (error) {
          lastError = error;
        }
      }

      if (attempt < _maxAttempts) {
        await Future<void>.delayed(const Duration(seconds: 1));
      }
    }

    throw Exception('Failed to fetch sensor data: $lastError');
  }
}