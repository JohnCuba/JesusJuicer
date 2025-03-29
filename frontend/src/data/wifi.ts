import useSWRV from 'swrv'

import { apiClient, type WifiMode } from './config'

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

export const useGetNetworkAP = () =>
  useSWRV('/wifi/network', apiClient.api.getNetworkApCredentials, { refreshInterval: 5000 })

export const useSetNetworkAP = () => {
  const { mutate } = useGetNetworkAP()

  return async (...data: Parameters<typeof apiClient.api.setNetworkApCredentials>) => {
    await apiClient.api.setNetworkApCredentials(...data)
    mutate()
  }
}

export const useDelNetworkAP = () => {
  const { mutate } = useGetNetworkAP()

  return async (...data: Parameters<typeof apiClient.api.delNetworkApCredentials>) => {
    await apiClient.api.delNetworkApCredentials(...data)
    mutate()
  }
}

export const useGetSelfAp = () => useSWRV('/wifi/ap', apiClient.api.getApCredentials)

export const useSetSelfAp = () => {
  const { mutate } = useGetSelfAp()

  return async (...data: Parameters<typeof apiClient.api.setApCredentials>) => {
    await apiClient.api.setApCredentials(...data)
    mutate()
  }
}

export const useGetWifiState = () => useSWRV('/wifi/state', apiClient.api.getWifiState)
