import useSWRV from 'swrv'
import { apiClient } from './config'
import { useUploadFile } from './config_upload'

export const useGetFwVersion = () =>
  useSWRV('/update/fw/version', () =>
    apiClient.api.getFirmwareVersion({
      // TODO: Api-fiddle generator not place it =(
      format: 'text',
    }),
  )

export const useUploadFw = () =>
  useUploadFile({
    url: 'http://juicer.local/api/update/fw',
  })

export const useUploadFs = () =>
  useUploadFile({
    url: 'http://juicer.local/api/update/fs',
  })
