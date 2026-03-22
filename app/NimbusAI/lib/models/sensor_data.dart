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
      rainSensorAdc: _readInt(json['rain_sensor_adc'] ?? json['rain_adc']),
      rainLevel: rainLevel,
      prediction: prediction,
      modelVersion: _readInt(json['model_version'] ?? json['modelVersion']),
      createdAt: _readString(json['created_at'] ?? json['createdAt']),
    );
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