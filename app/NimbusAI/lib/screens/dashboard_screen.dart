import 'dart:math' as math;

import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';

import '../models/sensor_data.dart';

class DashboardScreen extends StatefulWidget {
  const DashboardScreen({super.key});

  @override
  State<DashboardScreen> createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  static const String _databaseUrl =
      'https://rainfallprediction-945cd-default-rtdb.asia-southeast1.firebasedatabase.app';

  late final DatabaseReference _ref =
      FirebaseDatabase.instanceFor(
        app: Firebase.app(),
        databaseURL: _databaseUrl,
      ).ref('RainData');

  Future<void> _reloadData() async {
    setState(() {});
  }

  Map<String, dynamic> _toStringKeyMap(Map<dynamic, dynamic> source) {
    final mapped = <String, dynamic>{};
    for (final entry in source.entries) {
      mapped[entry.key.toString()] = entry.value;
    }
    return mapped;
  }

  Map<String, dynamic>? _extractLatestRecord(Map<String, dynamic> rainData) {
    final latestRaw = rainData['latest'];
    if (latestRaw is Map) {
      final latest = _toStringKeyMap(latestRaw);
      return _isRecordMap(latest) ? latest : null;
    }

    final historyRaw = rainData['history'];
    if (historyRaw is Map) {
      final historyMap = _toStringKeyMap(historyRaw);
      final fromHistory = _extractLatestRecord(historyMap);
      if (fromHistory != null) {
        return fromHistory;
      }
    }

    if (_isRecordMap(rainData)) {
      return rainData;
    }

    final records = <MapEntry<String, dynamic>>[];
    for (final entry in rainData.entries) {
      if (entry.value is Map) {
        records.add(MapEntry(entry.key, entry.value));
      }
    }

    if (records.isEmpty) {
      return null;
    }

    records.sort((a, b) {
      final aNum = int.tryParse(a.key);
      final bNum = int.tryParse(b.key);
      if (aNum != null && bNum != null) {
        return aNum.compareTo(bNum);
      }
      return a.key.compareTo(b.key);
    });

    final latestRaw = records.last.value;
    if (latestRaw is! Map) {
      return null;
    }

    final latest = _toStringKeyMap(latestRaw);
    return _isRecordMap(latest) ? latest : null;
  }

  bool _isRecordMap(Map<String, dynamic> data) {
    return data.containsKey('humidity') &&
        (data.containsKey('temperature') || data.containsKey('temperatur'));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          const _Backdrop(),
          SafeArea(
            child: RefreshIndicator(
              onRefresh: _reloadData,
              child: ListView(
                physics: const AlwaysScrollableScrollPhysics(),
                padding: const EdgeInsets.fromLTRB(16, 18, 16, 28),
                children: [
                  const _Header(),
                  const SizedBox(height: 18),
                  StreamBuilder<DatabaseEvent>(
                    stream: _ref.onValue,
                    builder: (context, snapshot) {
                      if (snapshot.connectionState == ConnectionState.waiting) {
                        return const Padding(
                          padding: EdgeInsets.only(top: 120),
                          child: Center(child: CircularProgressIndicator()),
                        );
                      }

                      if (snapshot.hasError) {
                        return _ErrorState(
                          onRetry: _reloadData,
                          message: snapshot.error.toString(),
                        );
                      }

                      final raw = snapshot.data?.snapshot.value;
                      if (raw == null || raw is! Map) {
                        return _ErrorState(
                          onRetry: _reloadData,
                          message: 'No sensor payload returned from Firebase.',
                        );
                      }

                      final latest = _extractLatestRecord(_toStringKeyMap(raw));
                      if (latest == null) {
                        return _ErrorState(
                          onRetry: _reloadData,
                          message: 'RainData contains no valid realtime record.',
                        );
                      }

                      final data = SensorData.fromJson(latest);

                      return TweenAnimationBuilder<double>(
                        duration: const Duration(milliseconds: 650),
                        curve: Curves.easeOutCubic,
                        tween: Tween(begin: 0, end: 1),
                        builder: (context, value, child) {
                          return Opacity(
                            opacity: value,
                            child: Transform.translate(
                              offset: Offset(0, (1 - value) * 12),
                              child: child,
                            ),
                          );
                        },
                        child: _DashboardContent(data: data),
                      );
                    },
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
      floatingActionButton: FloatingActionButton.extended(
        onPressed: () async => _reloadData(),
        icon: const Icon(Icons.sync),
        label: const Text('Refresh Data'),
      ),
    );
  }
}

class _Backdrop extends StatelessWidget {
  const _Backdrop();

  @override
  Widget build(BuildContext context) {
    return DecoratedBox(
      decoration: const BoxDecoration(
        gradient: LinearGradient(
          begin: Alignment.topLeft,
          end: Alignment.bottomRight,
          colors: [Color(0xFFEAF3FA), Color(0xFFF7FAFD), Color(0xFFEFF7F5)],
        ),
      ),
      child: Stack(
        children: [
          Positioned(
            top: -90,
            right: -70,
            child: _blob(const Color(0x443C84A2), 240),
          ),
          Positioned(
            top: 190,
            left: -50,
            child: _blob(const Color(0x33227A6A), 170),
          ),
          Positioned(
            bottom: -70,
            right: -20,
            child: _blob(const Color(0x22235E88), 190),
          ),
        ],
      ),
    );
  }

  Widget _blob(Color color, double size) {
    return Container(
      width: size,
      height: size,
      decoration: BoxDecoration(
        shape: BoxShape.circle,
        color: color,
      ),
    );
  }
}

class _Header extends StatelessWidget {
  const _Header();

  @override
  Widget build(BuildContext context) {
    final textTheme = Theme.of(context).textTheme;

    return Row(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        SizedBox(
          width: 64,
          height: 64,
          child: Image.asset(
            'assets/images/app_icon.png',
            fit: BoxFit.contain,
            filterQuality: FilterQuality.high,
          ),
        ),
        const SizedBox(width: 12),
        Expanded(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text('NimbusAI', style: textTheme.titleLarge),
              const SizedBox(height: 4),
              Text(
                'Creative meteorological dashboard for formal station reporting',
                style: textTheme.bodyMedium?.copyWith(
                  color: const Color(0xFF4F5D6D),
                  letterSpacing: 0.2,
                ),
              ),
            ],
          ),
        ),
      ],
    );
  }
}

class _DashboardContent extends StatelessWidget {
  const _DashboardContent({required this.data});

  final SensorData data;

  @override
  Widget build(BuildContext context) {
    final accent = _predictionColor(data.rainLevel);

    return Column(
      children: [
        _StatusCard(data: data, accent: accent),
        const SizedBox(height: 12),
        _MetricGrid(data: data),
        const SizedBox(height: 12),
        _TrendCard(data: data),
      ],
    );
  }

  static Color _predictionColor(int rainLevel) {
    switch (rainLevel) {
      case 2:
        return const Color(0xFF0E5D8A);
      case 1:
        return const Color(0xFF368DA6);
      default:
        return const Color(0xFF2E7C62);
    }
  }
}

class _StatusCard extends StatelessWidget {
  const _StatusCard({required this.data, required this.accent});

  final SensorData data;
  final Color accent;

  @override
  Widget build(BuildContext context) {
    final textTheme = Theme.of(context).textTheme;

    return Card(
      child: Container(
        padding: const EdgeInsets.all(16),
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(20),
          gradient: LinearGradient(
            begin: Alignment.topLeft,
            end: Alignment.bottomRight,
            colors: [
              const Color(0xFFFFFFFF),
              accent.withValues(alpha: 0.08),
            ],
          ),
        ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                _badge('Live'),
                const SizedBox(width: 8),
                _badge('Station A-01'),
                const Spacer(),
                Text(
                  'Intensity ${data.rainIntensity.toStringAsFixed(2)}',
                  style: textTheme.bodyMedium?.copyWith(
                    color: const Color(0xFF3C5063),
                    fontWeight: FontWeight.w600,
                  ),
                ),
              ],
            ),
            const SizedBox(height: 14),
            Row(
              children: [
                Container(
                  width: 78,
                  height: 78,
                  padding: const EdgeInsets.all(12),
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(18),
                    color: Colors.white,
                    border: Border.all(color: accent.withValues(alpha: 0.22)),
                  ),
                  child: Image.asset(_weatherImagePath(data.rainLevel)),
                ),
                const SizedBox(width: 14),
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        data.prediction,
                        style: textTheme.titleMedium?.copyWith(color: accent),
                      ),
                      const SizedBox(height: 6),
                      Text(
                        'Prediction validated from atmospheric sensor stream.',
                        style: textTheme.bodyMedium?.copyWith(
                          color: const Color(0xFF5D6A7A),
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }

  static Widget _badge(String text) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 10, vertical: 5),
      decoration: BoxDecoration(
        color: const Color(0xFFE8F1FA),
        borderRadius: BorderRadius.circular(999),
      ),
      child: Text(
        text,
        style: const TextStyle(
          fontSize: 12,
          fontWeight: FontWeight.w600,
          color: Color(0xFF2E5476),
        ),
      ),
    );
  }

  static String _weatherImagePath(int rainLevel) {
    switch (rainLevel) {
      case 2:
        return 'assets/images/rain.png';
      case 1:
        return 'assets/images/cloud.png';
      default:
        return 'assets/images/sun.png';
    }
  }
}

class _MetricGrid extends StatelessWidget {
  const _MetricGrid({required this.data});

  final SensorData data;

  @override
  Widget build(BuildContext context) {
    final metrics = [
      _MetricItem('Temperature', '${data.temperature.toStringAsFixed(1)} C', Icons.thermostat),
      _MetricItem('Humidity', '${data.humidity.toStringAsFixed(1)} %', Icons.water_drop),
      _MetricItem('SNR', data.snr.toStringAsFixed(2), Icons.graphic_eq),
      _MetricItem('Rain Intensity', data.rainIntensity.toStringAsFixed(2), Icons.grain),
    ];

    return LayoutBuilder(
      builder: (context, constraints) {
        final columns = constraints.maxWidth > 700 ? 4 : 2;
        final width = (constraints.maxWidth - (columns - 1) * 10) / columns;

        return Wrap(
          spacing: 10,
          runSpacing: 10,
          children: metrics
              .map(
                (metric) => SizedBox(
                  width: width,
                  child: _MetricCard(metric: metric),
                ),
              )
              .toList(),
        );
      },
    );
  }
}

class _MetricCard extends StatelessWidget {
  const _MetricCard({required this.metric});

  final _MetricItem metric;

  @override
  Widget build(BuildContext context) {
    final textTheme = Theme.of(context).textTheme;

    return Card(
      child: Padding(
        padding: const EdgeInsets.all(14),
        child: Row(
          children: [
            Container(
              width: 34,
              height: 34,
              decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(11),
                color: const Color(0xFFE6F2F8),
              ),
              child: Icon(metric.icon, size: 19, color: const Color(0xFF2D6286)),
            ),
            const SizedBox(width: 10),
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    metric.title,
                    style: textTheme.bodyMedium?.copyWith(
                      color: const Color(0xFF5B6778),
                      fontSize: 13,
                    ),
                  ),
                  const SizedBox(height: 2),
                  Text(metric.value, style: textTheme.titleMedium),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class _TrendCard extends StatelessWidget {
  const _TrendCard({required this.data});

  final SensorData data;

  @override
  Widget build(BuildContext context) {
    final textTheme = Theme.of(context).textTheme;
    final spots = _buildSpots(data);

    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text('6-Point Rainfall Pattern', style: textTheme.titleMedium),
            const SizedBox(height: 4),
            Text(
              'Short horizon trend simulation using humidity, SNR and intensity.',
              style: textTheme.bodyMedium?.copyWith(color: const Color(0xFF5F6B7C)),
            ),
            const SizedBox(height: 12),
            SizedBox(
              height: 165,
              child: LineChart(
                LineChartData(
                  minY: 0,
                  gridData: FlGridData(
                    show: true,
                    drawVerticalLine: false,
                    horizontalInterval: 1,
                    getDrawingHorizontalLine: (_) =>
                        const FlLine(color: Color(0xFFE7EEF4), strokeWidth: 1),
                  ),
                  titlesData: FlTitlesData(
                    leftTitles: const AxisTitles(
                      sideTitles: SideTitles(showTitles: false),
                    ),
                    rightTitles: const AxisTitles(
                      sideTitles: SideTitles(showTitles: false),
                    ),
                    topTitles: const AxisTitles(
                      sideTitles: SideTitles(showTitles: false),
                    ),
                    bottomTitles: AxisTitles(
                      sideTitles: SideTitles(
                        showTitles: true,
                        interval: 1,
                        getTitlesWidget: (value, _) => Text(
                          'T${value.toInt() + 1}',
                          style: textTheme.bodyMedium?.copyWith(
                            fontSize: 11,
                            color: const Color(0xFF7A8798),
                          ),
                        ),
                      ),
                    ),
                  ),
                  borderData: FlBorderData(show: false),
                  lineBarsData: [
                    LineChartBarData(
                      spots: spots,
                      isCurved: true,
                      barWidth: 3,
                      color: const Color(0xFF13638F),
                      dotData: const FlDotData(show: false),
                      belowBarData: BarAreaData(
                        show: true,
                        gradient: LinearGradient(
                          begin: Alignment.topCenter,
                          end: Alignment.bottomCenter,
                          colors: [
                            const Color(0xFF13638F).withValues(alpha: 0.26),
                            Colors.transparent,
                          ],
                        ),
                      ),
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  List<FlSpot> _buildSpots(SensorData data) {
    final base = math.max(0.2, data.rainIntensity);
    final humidityFactor = (data.humidity / 100).clamp(0.3, 1.0);
    final snrFactor = (data.snr / 20).clamp(0.2, 1.2);

    return List<FlSpot>.generate(6, (index) {
      final wave = math.sin(index * 0.9) * 0.35;
      final trend = index * 0.15;
      final y = base * humidityFactor * snrFactor + wave + trend;
      return FlSpot(index.toDouble(), y);
    });
  }
}

class _ErrorState extends StatelessWidget {
  const _ErrorState({required this.onRetry, required this.message});

  final Future<void> Function() onRetry;
  final String message;

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(20),
        child: Column(
          children: [
            Container(
              width: 64,
              height: 64,
              padding: const EdgeInsets.all(12),
              decoration: BoxDecoration(
                color: const Color(0xFFE8F2FA),
                borderRadius: BorderRadius.circular(16),
              ),
              child: Image.asset('assets/images/cloud.png'),
            ),
            const SizedBox(height: 12),
            Text(
              message,
              textAlign: TextAlign.center,
              style: Theme.of(context).textTheme.bodyMedium,
            ),
            const SizedBox(height: 12),
            FilledButton.icon(
              onPressed: () => onRetry(),
              icon: const Icon(Icons.refresh),
              label: const Text('Retry'),
            ),
          ],
        ),
      ),
    );
  }
}

class _MetricItem {
  const _MetricItem(this.title, this.value, this.icon);

  final String title;
  final String value;
  final IconData icon;
}
