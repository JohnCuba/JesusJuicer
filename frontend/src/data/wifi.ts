import useSWRV from 'swrv'
import { apiClient } from './config'
import { onMounted } from 'vue'

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
