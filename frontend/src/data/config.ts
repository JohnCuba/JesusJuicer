import { Api } from './config_api'
export type { WifiMode } from './config_api'

export const apiClient = new Api({
  baseUrl: 'http://juicer.local',
})
