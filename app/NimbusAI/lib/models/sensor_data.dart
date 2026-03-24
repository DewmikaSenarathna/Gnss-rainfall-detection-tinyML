class SensorData {
  final double temperature;
  final double humidity;
  final double snr;
  final int maxSnr;
  final int snrSamples;
  final double rainIntensity;
  final int rainSensorAdc;
  final int rainLevel;
  final String prediction;
  final int modelVersion;
  final String createdAt;

  SensorData({
    required this.temperature,
    required this.humidity,
    required this.snr,
    required this.maxSnr,
    required this.snrSamples,
    required this.rainIntensity,
    required this.rainSensorAdc,
    required this.rainLevel,
    required this.prediction,
    required this.modelVersion,
    required this.createdAt,
  });

  factory SensorData.fromJson(Map<String, dynamic> json) {
    final rainLevel = _readInt(json['rainLevel'] ?? json['rain_level']);
    final prediction = _readPrediction(
      json['prediction'],
      rainLevel,
    );

    return SensorData(
      temperature: _readDouble(json['temperature'] ?? json['temperatur']),
      humidity: _readDouble(json['humidity']),
      snr: _readDouble(json['snr'] ?? json['maxSNR'] ?? json['max_snr']),
      maxSnr: _readInt(json['maxSNR'] ?? json['max_snr']),
      snrSamples: _readInt(json['snrSamples'] ?? json['snr_samples']),
      rainIntensity: _readDouble(
        json['rain_intensity'] ?? json['rainValue'] ?? json['rain_sensor'],
      ),
      rainSensorAdc: _readRainSensorAdc(json),
      rainLevel: rainLevel,
      prediction: prediction,
      modelVersion: _readInt(json['model_version'] ?? json['modelVersion']),
      createdAt: _readString(json['created_at'] ?? json['createdAt']),
    );
  }

  static int _readRainSensorAdc(Map<String, dynamic> json) {
    // Prefer direct rain sensor value keys first for faster and fresher updates.
    final preferred = [
      json['rainValue'],
      json['rain_sensor'],
      json['rain_sensor_adc'],
      json['rain_adc'],
    ];

    for (final value in preferred) {
      final parsed = _readInt(value);
      if (value != null || parsed != 0) {
        return parsed;
      }
    }

    return 0;
  }

  Map<String, dynamic> toJson() {
    return {
      'temperature': temperature,
      'humidity': humidity,
      'snr': snr,
      'maxSNR': maxSnr,
      'snrSamples': snrSamples,
      'rain_intensity': rainIntensity,
      'rain_sensor_adc': rainSensorAdc,
      'rainLevel': rainLevel,
      'prediction': prediction,
      'model_version': modelVersion,
      'created_at': createdAt,
    };
  }

  String get createdAtOrNow {
    if (createdAt.trim().isNotEmpty) {
      return createdAt;
    }
    return DateTime.now().toIso8601String();
  }

  static double _readDouble(dynamic value) {
    if (value is num) {
      return value.toDouble();
    }
    if (value is String) {
      return double.tryParse(value) ?? 0;
    }
    return 0;
  }

  static int _readInt(dynamic value) {
    if (value is int) {
      return value;
    }
    if (value is num) {
      return value.round();
    }
    if (value is String) {
      return int.tryParse(value) ?? 0;
    }
    return 0;
  }

  static String _readPrediction(dynamic prediction, int rainLevel) {
    if (prediction != null && prediction.toString().trim().isNotEmpty) {
      return prediction.toString();
    }

    switch (rainLevel) {
      case 0:
        return 'No Rain';
      case 1:
        return 'Low Rain';
      case 2:
        return 'High Rain';
      default:
        return 'Unknown';
    }
  }

  static String _readString(dynamic value) {
    if (value == null) {
      return '';
    }
    return value.toString();
  }
}