import 'dart:async';
import 'dart:convert';

import 'package:firebase_database/firebase_database.dart';

import '../models/sensor_data.dart';
import 'api_service.dart';

class RealtimeSyncService {
  RealtimeSyncService({
    required DatabaseReference databaseRef,
    ApiService? apiService,
    this.interval = const Duration(seconds: 3),
    this.onSyncStatusChanged,
  })  : _databaseRef = databaseRef,
        _apiService = apiService ?? ApiService();

  final DatabaseReference _databaseRef;
  final ApiService _apiService;
  final Duration interval;
  final void Function(String? message)? onSyncStatusChanged;

  Timer? _timer;
  bool _isSyncRunning = false;
  String? _lastPayloadHash;

  Future<void> start() async {
    if (_timer != null) {
      return;
    }

    unawaited(syncOnce());
    _timer = Timer.periodic(interval, (_) {
      unawaited(syncOnce());
    });
  }

  void stop() {
    _timer?.cancel();
    _timer = null;
  }

  Future<SensorData?> syncOnce() async {
    if (_isSyncRunning) {
      return null;
    }

    _isSyncRunning = true;
    try {
      final data = await _apiService.fetchData();
      await _storeInFirebase(data);
      _emitSyncStatus(null);
      return data;
    } on Exception catch (error) {
      _emitSyncStatus(error.toString());
      return null;
    } finally {
      _isSyncRunning = false;
    }
  }

  void _emitSyncStatus(String? message) {
    onSyncStatusChanged?.call(message);
  }

  Future<void> _storeInFirebase(SensorData data) async {
    final record = <String, dynamic>{
      ...data.toJson(),
      'created_at': data.createdAtOrNow,
    };

    final hash = jsonEncode(record);
    if (hash == _lastPayloadHash) {
      return;
    }

    _lastPayloadHash = hash;

    final timestamp = DateTime.now().millisecondsSinceEpoch.toString();
    await _databaseRef.update({
      'latest': record,
      'history/$timestamp': {
        ...record,
        'stored_at': ServerValue.timestamp,
      },
    });
  }
}
