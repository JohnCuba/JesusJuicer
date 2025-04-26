import useSWRV from 'swrv'
import { apiClient } from './config'

export const useGetTelegramChatId = () =>
  useSWRV('/telegram/chat_id', () => apiClient.api.getTelegramChatId({ format: 'text' }))

export const useSetTelegramChatId = () => {
  const { mutate } = useGetTelegramChatId()

  return async (...data: Parameters<typeof apiClient.api.setTelegramChatId>) => {
    await apiClient.api.setTelegramChatId(...data)
    mutate()
  }
}

export const usePingTelegram = () => apiClient.api.sendPingToTelegramBot
