import useSWRV from 'swrv'

import { apiClient } from './config'

export const useGetTds = () =>
  useSWRV('/tds', () =>
    apiClient.api.getTdsSensorValue({
      // TODO: Api-fiddle generator not place it =(
      format: 'text',
    }),
  )
