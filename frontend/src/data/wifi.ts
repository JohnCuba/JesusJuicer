import useSWRV from 'swrv'
import { apiClient } from './config'
import { onMounted } from 'vue'

export { wifiModes } from './config'

export const useGetNetworkAP = () => {
  onMounted(() => {
    apiClient.api.preflightRequest({
      headers: {
        'Access-Control-Request-Method': 'GET, POST, PATCH, DELETE, OPTIONS',
      },
    })
  })

  return useSWRV('/wifi/network', apiClient.api.getNetworkApCredentials, { refreshInterval: 5000 })
}

export const useSetNetworkAP = () => {
  const { mutate } = useSWRV('/wifi/network', null)

  return async (...data: Parameters<typeof apiClient.api.setNetworkApCredentials>) => {
    await apiClient.api.setNetworkApCredentials(...data)
    mutate()
  }
}

export const useDelNetworkAP = () => {
  const { mutate } = useSWRV('/wifi/network', null)

  return async (...data: Parameters<typeof apiClient.api.delNetworkApCredentials>) => {
    await apiClient.api.delNetworkApCredentials(...data)
    mutate()
  }
}

export const useGetSelfAp = () => {
  return useSWRV('/wifi/ap', apiClient.api.getApCredentials)
}

export const useSetSelfAp = () => {
  const { mutate } = useSWRV('/wifi/ap', null)

  return async (...data: Parameters<typeof apiClient.api.setApCredentials>) => {
    await apiClient.api.setApCredentials(...data)
    mutate()
  }
}

export const useGetWifiState = () => useSWRV('/wifi/state', apiClient.api.getWifiState)
