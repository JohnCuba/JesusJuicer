import useSWRV from 'swrv'
import { apiClient } from './config'

export const useWifiList = () => useSWRV('/wifi/list', apiClient.api.wifiList)

export const useUpdateWifi = () => {
  const { mutate } = useSWRV('/wifi/list', null)

  return async (...data: Parameters<typeof apiClient.api.wifiUpdate>) => {
    await apiClient.api.wifiUpdate(...data)
    mutate()
  }
}
