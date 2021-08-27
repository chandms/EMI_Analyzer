import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { RouterModule } from '@angular/router';
import { ChartsModule } from 'ng2-charts';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SweepComponent } from './sweep/sweep.component';
import { SweepService } from './service/sweep.service';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { DeviceComponent } from './device/device.component';
import { DeviceService } from './service/device.service';
import { LocationComponent } from './location/location.component';
import { MapAllComponent } from './map-all/map-all.component';
import { StrengthChartComponent } from './strength-chart/strength-chart.component';
import { LoginComponent } from './login/login.component';
import { LoginService } from './service/login.service';
import { ReactiveFormsModule } from '@angular/forms';

@NgModule({
  declarations: [
    AppComponent,
    SweepComponent,
    DeviceComponent,
    LocationComponent,
    MapAllComponent,
    StrengthChartComponent,
    LoginComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    AppRoutingModule,
    HttpClientModule,
    BrowserAnimationsModule,
    ChartsModule,
    ReactiveFormsModule,
    RouterModule.forRoot([
      {
        path: '',
        component: LoginComponent
      },
      {
        path: 'device/:deviceName',
        component: DeviceComponent
      },
      {
        path: 'location/:deviceName',
        component: LocationComponent
      },
      {
        path: 'map-devices',
        component: SweepComponent
      },
      
    ])
  ],
  providers: [
    SweepService,
    DeviceService,
    LoginService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
