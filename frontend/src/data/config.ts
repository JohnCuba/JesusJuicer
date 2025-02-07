import { Api, type WifiMode } from './config_api'

export const apiClient = new Api({
  baseUrl: 'http://aqua.local',
})

export const wifiModes: Record<WifiMode, string> = {
  '0': 'Off',
  '1': 'station',
  '2': 'soft-AP',
  '3': 'station + soft-AP',
} as const

export const wifiSignalQuality = ['no_signal', 'very_low', 'low', 'good', 'very_good'] as const

export const RSSItoWifiSignalQualityIndex = (rssi: number = -1000) => {
  if (rssi >= -60) return 4
  else if (rssi >= -70) return 3
  else if (rssi >= -90) return 2
  else if (rssi >= -100) return 1
  return 0
}
