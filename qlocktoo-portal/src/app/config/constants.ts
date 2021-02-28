import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class Constants {
    public readonly API_ENDPOINT: string = 'http://192.168.0.111/api';
    public readonly API_MOCK_ENDPOINT: string = 'mock-domain/api';
}
