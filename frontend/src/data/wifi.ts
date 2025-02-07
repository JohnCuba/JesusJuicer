import useSWRV from 'swrv'
import { apiClient } from './config'
import { onMounted } from 'vue'

export { wifiModes } from './config'

export const useWifiList = () => {
  onMounted(() => {
    apiClient.api.preflightRequest({
      headers: {
        'Access-Control-Request-Method': 'GET, POST, PATCH, DELETE, OPTIONS',
      },
    })
  })

  return useSWRV('/wifi/list', apiClient.api.getSavedCredentials, { refreshInterval: 5000 })
}

export const useSaveWifi = () => {
  const { mutate } = useSWRV('/wifi/list', null)

  return async (...data: Parameters<typeof apiClient.api.saveCredentials>) => {
    await apiClient.api.saveCredentials(...data)
    mutate()
  }
}

export const useEditWifi = () => {
  const { mutate } = useSWRV('/wifi/list', null)

  return async (...data: Parameters<typeof apiClient.api.editSavedCredentials>) => {
    await apiClient.api.editSavedCredentials(...data)
    mutate()
  }
}

export const useDeleteWifi = () => {
  const { mutate } = useSWRV('/wifi/list', null)

  return async (...data: Parameters<typeof apiClient.api.removeSavedCredentials>) => {
    await apiClient.api.removeSavedCredentials(...data)
    mutate()
  }
}

export const useGetAp = () => {
  return useSWRV('/wifi/ap', apiClient.api.getApCredentials)
}

export const useSetAp = () => {
  const { mutate } = useSWRV('/wifi/ap', null)

  return async (...data: Parameters<typeof apiClient.api.setApCredentials>) => {
    await apiClient.api.setApCredentials(...data)
    mutate()
  }
}

export const useGetWifiState = () => useSWRV('/wifi/state', apiClient.api.getWifiState)
